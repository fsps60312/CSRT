#include<common/pod/pod_body.hpp>
namespace pod {
	glm::dmat4 PodBody::GetMatrixY()const {
		return matrix::RotateD(glm::dvec3(0, -1, 0), rotation_y);
	}
	glm::dmat4 PodBody::GetMatrixZ()const {
		//std::clog << rb.theta << std::endl;
		return matrix::RotateD(glm::dvec3(0, 0, 1), rb.theta);
	}
	glm::dmat4 PodBody::GetMatrixT()const {
		return matrix::TranslateD(rb.position);
	}
	RigidBody* PodBody::GetRigidBody() {
		return &rb;
	}
	void PodBody::ApplyTranslate(const glm::dvec3& offset) {
		rb.position += offset;
	}
	void PodBody::PrepareForRound() {
		rb.Reset();
	}
	double PodBody::GetCorrectiveAlpha()const {
		const double t = -10 * rb.theta - rb.omega;
		return std::pow(std::abs(t), 1) * t - 1 * std::abs(rb.omega) * rb.omega - 1 * rb.omega;
	}
	void PodBody::UpdateRotationZ() {
		if (!pod->IsOnGround()) {
			rb.alpha += GetCorrectiveAlpha();
			if (environment::IsKeyDown(GLFW_KEY_A))rb.alpha += 5;
			if (environment::IsKeyDown(GLFW_KEY_D))rb.alpha -= 5;
		}
	}
	void PodBody::UpdateRotationY() {
		if (!pod->IsOnGround() || true) {
			if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D))desired_rotation_y = PI;
			if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A))desired_rotation_y = 0;
		}
	}
	glm::dvec3 PodBody::GetForce()const {
		glm::dvec3 ret(0);
		ret += glm::dvec3(0, -rb.mass * constants::gravity, 0);
		ret += glm::dvec3(-std::sin(rb.theta) * propeller->GetLiftForce(), std::cos(rb.theta) * propeller->GetLiftForce(), 0);
		return ret;
	}
	void PodBody::UpdateRigidBody() {
		rb.position.z = rb.velocity.z = 0;
		rb.force += GetForce();
	}
	PodBody::DigIntention PodBody::GetDigIntention()const {
		if (!pod->IsPodStableOnGround()) {
			drill->SetFoldTarget(pod->IsOnGround() ? 1 : 0);
			return DigIntention::None;
		}
		if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D)) {
			drill->SetFoldTarget(1);
			return drill->GetFoldState() == 1 ? DigIntention::Left : DigIntention::None;
		}
		if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A)) {
			drill->SetFoldTarget(1);
			return drill->GetFoldState() == 1 ? DigIntention::Right : DigIntention::None;
		}
		if (environment::IsKeyDown(GLFW_KEY_S) && !environment::IsKeyDown(GLFW_KEY_W)) {
			drill->SetFoldTarget(-1);
			return drill->GetFoldState() == -1 ? DigIntention::Down : DigIntention::None;
		}
		drill->SetFoldTarget(pod->IsOnGround() ? 1 : 0);
		return DigIntention::None;
	}
	void PodBody::UpdateDigIntention(const double secs) {
		if (block_digging == NULL) {
			const DigIntention dig_intention = GetDigIntention();
			if (dig_intention == DigIntention::Left || dig_intention == DigIntention::Right) {
				block::Block* b = block_digging = pod->GetCollideFront();
				if (b != NULL) {
					b->SetDigState(dig_intention == DigIntention::Left ? block::Block::DigDirection::Left : block::Block::DigDirection::Right, 0);
					//std::clog << "dig dig dig!\n";
				}
			}
			if (dig_intention == DigIntention::Down) {
				block::Block* b = block_digging = pod->GetCollideDown();
				if (b != NULL) {
					b->SetDigState(block::Block::DigDirection::Down, 0);
				}
			}
		} else {
			const double dig_progress = block_digging->GetDigProgress();
			const double next_dig_progress = std::min(1.0, dig_progress + secs);
			const auto dig_direction = block_digging->GetDigDirection();
			block_digging->SetDigState(dig_direction, next_dig_progress);
			switch (dig_direction) {
			case block::Block::DigDirection::Down: {
				pod->ApplyTranslate(glm::dvec3(0, -constants::block_height * (next_dig_progress - dig_progress), 0));
				break;
			}
			case block::Block::DigDirection::Left: {
				pod->ApplyTranslate(glm::dvec3(-constants::block_width * (next_dig_progress - dig_progress), 0, 0));
				break;
			}
			case block::Block::DigDirection::Right: {
				pod->ApplyTranslate(glm::dvec3(constants::block_width * (next_dig_progress - dig_progress), 0, 0));
				break;
			}
			}
			if (block_digging->GetDigProgress() == 1) {
				block::Destroy(block_digging);
				block_digging = NULL;
			}
		}
	}
	void PodBody::UpdateFume(const double secs) {
		const glm::dvec3& pos = glm::dvec3(-body_radius, body_radius, mylib::Rand::NextDouble() - 0.5);
		const glm::dvec3& speed = glm::dvec3(-mylib::Rand::NextDouble() * 0.5 - 0.1, 1.0 + mylib::Rand::NextDouble() * 0.5, 0);
		effects::Fume::AddFume(matrix::Multiply(GetMatrixT() * GetMatrixZ() * GetMatrixY(), pos), matrix::Multiply(GetMatrixY(), speed), 0.1, 1.5, 0.5, 3);
	}
	void PodBody::Update(const double secs) {
		//if (pod->IsPodStableOnGround())std::clog << "stable!" << std::endl;
		UpdateDigIntention(secs);
		UpdateRotationY();
		UpdateRotationZ();
		UpdateRigidBody();
		UpdateFume(secs);
		for (auto& ch : children)ch->Update(secs);
	}
	void PodBody::AdvanceCamera(const double secs) {
		const double look_offset = 1.3;
		glm::dvec3 camera_position = camera::GetPosition();
		glm::dvec3 camera_direction = camera::GetDirection();
		glm::dvec3 t = rb.position - camera_position;
		t.z = 0;
		mylib::SmoothTo(camera_position, rb.position + glm::dvec3(0, 0, 30 / std::pow(0.4 + glm::length(t) * 0.1, 0.5)), secs, 0.2);
		glm::dvec3 target = rb.position + 0.1 * rb.velocity - camera_position;
		assert(target.z != 0);
		target /= std::abs(target.z);
		const double len = std::sqrt(std::pow(target.x, 2) + std::pow(target.y, 2));
		const double target_len = std::min(len, look_offset);
		if (target_len == 0) {
			target.x = target.y = 0;
		} else {
			target.x *= target_len / len;
			target.y *= target_len / len;
		}
		mylib::SmoothTo(camera_direction, target, secs, 0.2);
		camera::SetPosition(camera_position);
		camera::SetDirection(camera_direction);
	}
	bool PodBody::IsRigidBodyMoveTooMuch(const double secs)const {
		//return true;
		if (secs > 1e-3) return true;
		const double dif = glm::length(rb.position - rb._position);
		if (dif > body_radius / 5) return true;
		return false;
	}
	void PodBody::AdvanceRigidBody(const double secs) {
		rb.Advance(secs);
		if (IsRigidBodyMoveTooMuch(secs)) {
			rb.Restore();
			AdvanceRigidBody(secs / 2);
			AdvanceRigidBody(secs / 2);
		} else {
			return;
		}
	}
	void PodBody::Advance(const double secs) {
		propeller->Advance(secs);
		mylib::SmoothTo(rotation_y, desired_rotation_y, secs, std::max(std::abs(desired_rotation_y - rotation_y) / PI, 0.2) * 0.2);
		//const glm::dvec3 v = rb.force / rb.mass;
		//std::clog << "body.rb: " << v.x << "," << v.y << "," << v.z << std::endl;
		AdvanceRigidBody(secs);
		AdvanceCamera(secs);
		const glm::dmat4& mat_t = GetMatrixT(), & mat_z = GetMatrixZ(), & mat_y = GetMatrixY();
		SetTransform(mat_t * mat_z * mat_y);
	}
	const double PodBody::body_radius = 1.5;
	std::vector<Triangle> PodBody::GetTriangles()const {
		return Triangle::Cube(glm::vec3(body_radius));
	}
	PodBody::PodBody(PodInterface* pod) :
		pod(pod),
		propeller(new PodPropeller(pod, PodPropeller::Types::Basic, matrix::TranslateD(glm::dvec3(0, 1.5, 0))* matrix::RotateD(glm::dvec3(1, 0, 0), -PI / 2))),
		drill(new PodDrill(pod, 1.5, 50, matrix::TranslateD(glm::dvec3(1.5, 0, 0))* matrix::RotateD(glm::dvec3(0, 1, 0), 90.0 / 180 * PI))) {
		rb.mass = 0.8;
		//rb.theta = 0.2;
		children.insert(new VisibleObject(GetTriangles()));
		children.insert(propeller);
		children.insert(drill);
	}
}