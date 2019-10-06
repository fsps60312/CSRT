#include<common/pod/pod_tracks.hpp>
namespace pod {
	void PodTracks::Track::Gear::Rotate(const double len) {
		rb.theta += len / radius;
	}
	bool PodTracks::Track::Gear::IsOnGround() const{
		return on_ground_countdown > 0;
	}
	void PodTracks::Track::Gear::PrepareForRound() {
		rb.Reset();
	}
	void PodTracks::Track::Gear::ApplyReactForceWithPod() {
		const glm::dvec3& f = GetReactForce();
		///minimize: (px+a*fx)^2+(py+a*fy)^2
		///2(px+a*fx)*fx+2(py+a*fy)*fy=0
		///px*fx+a*fx*fx+py*fy+a*fy*fy=0
		///a=-(px*fx+py*fy)/(fx*fx+fy*fy)
		if ((f.x * f.x + f.y * f.y) > 1e-9)
		{
			const glm::dvec3 &p = GetDesiredPosition() - pod->GetRigidBody()->position;
			const double a = -(p.x * f.x + p.y * f.y) / (f.x * f.x + f.y * f.y);
			const glm::dvec3 &forceArm = p + f * a;
			const double torque = glm::cross(forceArm, f).z;
			//System.Diagnostics.Trace.WriteLine($"torque: {torque}");
			pod->GetRigidBody()->alpha += torque;
		}
		pod->GetRigidBody()->force += f;
		rb.force -= f;
	}
	void PodTracks::Track::Gear::Update(const double secs) {
		ApplyReactForceWithPod();
		rb.force += glm::dvec3(0, -rb.mass * constants::gravity, 0);
	}
	void PodTracks::Track::Gear::RotateYAlongWithPod() {
		const glm::dmat4& mat_y = pod->GetMatrixY(), & mat_z = pod->GetMatrixZ(), & mat_t = pod->GetMatrixT();
		const glm::dmat4& mat_trans = mat_t * mat_z * mat_y * matrix::Inverse(mat_t * mat_z * pre_matrix_y);
		//const glm::dvec3& r_pos = matrix::Multiply(matrix::Inverse(mat_t * mat_z * pre_matrix_y), rb.position);
		//std::clog << "\t" << r_pos.x << " / " << relative_position.x << "\t" << r_pos.y << " / " << relative_position.y << "\t" << r_pos.z << " / " << relative_position.z << std::endl;
		const glm::dvec3& new_position = matrix::Multiply(mat_trans, rb.position);
		//const glm::dvec3& new_velocity = matrix::Multiply(mat_trans, rb.velocity);
		//RB.position.X = newPosition.X; RB.position.Z = newPosition.Z;
		rb.position = new_position;
		//rb.velocity = new_velocity;
		//RB.velocity.X = newVelocity.X; RB.velocity.Y = newVelocity.Y;
		pre_matrix_y = mat_y;
		pre_matrix_z = mat_z;
	}
	bool PodTracks::Track::Gear::IsRigidBodyMoveTooMuch(const double secs) const {
		if (secs > 1e-3) return true;
		const double dif = glm::length(rb.position - rb._position);
		if (dif > radius / 5) return true;
		return false;
	}
	void PodTracks::Track::Gear::MoveGearBackToTouchSurface(const glm::dvec3& offset) {
		double l = 0, r = 1;
		while (r - l > 1e-9) {
			double mid = (l + r) / 2;
			if (block::IsCollidable(rb.position + offset * mid))r = mid;
			else l = mid;
		}
		rb.position += offset * (-1 + r);
	}
	void PodTracks::Track::Gear::InverseVelocityIfCollideWithBlocks() {
		glm::dvec3 ret = glm::dvec3(0);
		if (block::IsCollidable(rb.position))return; // inside a block, no respond
		//bool rollback = false;
		const double rollback_speed = 0.1;
		const double bounce_coe = 0.1;
		const double friction_coe = 0.5;
		for (const glm::dvec3& offset : { glm::dvec3(-radius,0,0),glm::dvec3(radius,0,0),glm::dvec3(0,-radius,0),glm::dvec3(0,radius,0) }) {
			const glm::dvec3& dir = glm::normalize(offset);
			if (block::IsCollidable(rb.position + offset)) {
				MoveGearBackToTouchSurface(offset);
				const double sub_velocity_length = glm::dot(rb.velocity, dir);
				if (sub_velocity_length > 0) {
					if (std::abs(-1 - dir.y) < 1e-9)on_ground_countdown = 0.1; // downward collision
					const double dv = (1.0 + bounce_coe) * sub_velocity_length;
					rb.velocity -= dir * dv;
					const glm::dvec3& hori_speed_dir = glm::cross(dir, glm::dvec3(0, 0, 1));
					glm::dvec3& speed_include_rotate = rb.velocity + matrix::Multiply(pod->GetMatrixY(), glm::normalize(glm::cross(dir, glm::dvec3(0, 0, 1)))) * track->GetTrackSpeed();
					speed_include_rotate.z = 0;
					const glm::dvec3& hori_speed_vec = glm::dot(hori_speed_dir, speed_include_rotate) * hori_speed_dir;
					const glm::dvec3& friction_dv = -glm::normalize(hori_speed_vec) * dv * friction_coe;
					if (glm::length(friction_dv) > glm::length(hori_speed_vec))rb.velocity -= hori_speed_vec;
					else rb.velocity += friction_dv;
				}
			}
		}
	}
	void PodTracks::Track::Gear::AdvanceRigidBody(const double secs) {
		rb.Advance(secs);
		if (IsRigidBodyMoveTooMuch(secs))
		{
			rb.Restore();
			AdvanceRigidBody(secs / 2);
			AdvanceRigidBody(secs / 2);
		} else {
			on_ground_countdown -= secs;
			InverseVelocityIfCollideWithBlocks();
			//rb.Restore();
			//const glm::dvec3& react_force = GetReactForceWithBlocks(secs);
			//rb.Advance(secs);
		}
	}
	void PodTracks::Track::Gear::Advance(const double secs) {
		Rotate(secs * track->GetTrackSpeed());
		RotateYAlongWithPod();
		//const glm::dvec3 v = rb.force / rb.mass;
		//std::clog << "gear.rb: " << v.x << "," << v.y << "," << v.z << std::endl;
		//auto prep=rb.position;
		AdvanceRigidBody(secs);
		//rb.position = prep;
		//rb.velocity = glm::dvec3(0.0);
		SetTransform(matrix::TranslateD(rb.position) * pod->GetMatrixY() * matrix::RotateD(glm::dvec3(0, 0, -1), rb.theta));
	}
	glm::dvec3 PodTracks::Track::Gear::GetReactForce()const {
		const glm::dvec3 on_pod_velocity = pod->GetRigidBody()->GetVelocityAt(matrix::Multiply(pod->GetMatrixY(), relative_position));
		const glm::dvec3&
			elastic_force = suspension_hardness * (GetPosition() - GetDesiredPosition()),
			damp_force = -0.5 * (on_pod_velocity - rb.velocity);
		return elastic_force + damp_force;
	}
	glm::dvec3 PodTracks::Track::Gear::GetDesiredPosition()const {
		return  matrix::Multiply(pod->GetMatrixT() * pod->GetMatrixZ() * pod->GetMatrixY(), relative_position);
	}
	glm::dvec3 PodTracks::Track::Gear::GetPosition()const {
		return rb.position;
	}
	std::vector<Triangle>PodTracks::Track::Gear::GetTriangles(const double radius)const {
		const int n = 10;
		std::vector<glm::dvec3> positions;
		for (int i = 0; i < n; i++) positions.push_back(glm::dvec3(radius * std::cos(2 * PI * i / n), radius * std::sin(2 * PI * i / n), 0));
		std::vector<Triangle>ret;
		for (int i = 2; i < n; i++)if(i!=5) {
			ret.push_back(Triangle(glm::mat3(positions[0], positions[i - 1LL], positions[i])));
			ret.push_back(Triangle(glm::mat3(positions[i], positions[i - 1LL], positions[0])));
		}
		return ret;
	}
	PodTracks::Track::Gear::Gear(const glm::dvec3& relative_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod, Track* track) :
		relative_position(relative_position),
		radius(radius),
		suspension_hardness(suspension_hardness),
		mass(mass),
		pod(pod),
		track(track),
		pre_matrix_y(pod->GetMatrixY()),
		VisibleObject(GetTriangles(radius)) {
		rb = RigidBody();
		rb.mass = mass;
		rb.position = GetDesiredPosition();
		SetTransform(matrix::TranslateD(relative_position));
	}
}