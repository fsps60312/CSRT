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
	void PodBody::UpdateRotationZ() {
		if (!pod->IsOnGround()) {
			const double t = -10 * rb.theta - rb.omega;
			rb.alpha += std::pow(std::abs(t), 1) * t;
			if (environment::IsKeyDown(GLFW_KEY_A))rb.alpha += 5;
			if (environment::IsKeyDown(GLFW_KEY_D))rb.alpha -= 5;
			rb.alpha += -1 * std::abs(rb.omega) * rb.omega - 1 * rb.omega;
		}
	}
	void PodBody::UpdateRotationY() {
		if ((!pod->IsOnGround() || pod->IsPodStopped())) {
			if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D))desired_rotation_y = PI;
			if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A))desired_rotation_y = 0;
		}
	}
	void PodBody::UpdateRigidBody() {
		rb.position.z = rb.velocity.z = 0;
		rb.force += glm::dvec3(0, -rb.mass * constants::gravity, 0);
		rb.force += glm::dvec3(-std::sin(rb.theta) * propeller->GetLiftForce(), std::cos(rb.theta) * propeller->GetLiftForce(), 0);
	}
	void PodBody::Update(const double secs) {
		UpdateRotationY();
		UpdateRotationZ();
		UpdateRigidBody();
	}
	void PodBody::AdvanceCamera(const double secs) {
		const double look_offset = 1.3;
		glm::dvec3 camera_position = camera::GetPosition();
		glm::dvec3 camera_direction = camera::GetDirection();
		glm::dvec3 t = rb.position - camera_position;
		t.z = 0;
		mylib::SmoothTo(camera_position, rb.position + glm::dvec3(0, 0, 30 / std::pow(0.4 + glm::length(t) * 0.1, 0.5))/5.0, secs, 0.2);
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
		//int cur_x, cur_y, x = 0, y = 0;
		//Blocks.IsCollidable(rb.position, out cur_x, out cur_y);
		//if (secs < 1e-4)//accurate enough
		//{
		//	for (int cpi = -1; ;)
		//	{
		//		cpi = this.CollidePoints.FindIndex(cpi + 1, p = > Blocks.IsCollidable(rb.position + new Vector3D(Math.Cos(rb.theta) * p.X - Math.Sin(rb.theta) * p.Y, Math.Cos(rb.theta) * p.Y + Math.Sin(rb.theta) * p.X, p.Z), out x, out y));
		//		if (cpi == -1)
		//		{
		//			//{
		//			//    if (!tracks.UpdateRigidBody(secs, Math.Cos(RotationY), RB.velocity - RB._velocity, RB.theta, out Vector3D reactionForce, out double reactionTorque)) return false;
		//			//    //RB.force += reactionForce;
		//			//    //RB.alpha += reactionTorque;
		//			//}
		//			return true;
		//		}
		//		var cp = this.CollidePoints[cpi];
		//		const double bounce = 0.5;
		//		double t;
		//		t = cp.X * -Math.Sin(rb.theta) + cp.Y * -Math.Cos(rb.theta);
		//		if (x == cur_x - 1 && !Blocks.IsCollidable(cur_x, y) && rb.velocity.X + t * rb.omega < 0)//collide left, +x force. t=(cp.x*-Sin(theta)+cp.y*-Cos(theta)), (v.x+f/m) + (omega+f*t/I)*t = -b*(v.x + omega*t), f*(1/m+t^2/I)=(-b-1)*(v.x+omega*t)
		//		{
		//			var f = (-bounce - 1) * (rb.velocity.X + rb.omega * t) / (1.0 / rb.mass + t * t / rb.momentOfInertia);
		//			//f += -rb.force.X * secs;
		//			rb.velocity.X += f / rb.mass;
		//			rb.omega += f * t / rb.momentOfInertia;
		//		}
		//		t = cp.X * -Math.Sin(rb.theta) + cp.Y * -Math.Cos(rb.theta);
		//		if (x == cur_x + 1 && !Blocks.IsCollidable(cur_x, y) && rb.velocity.X + t * rb.omega > 0)//collide right, -x force. t=(cp.x*-Sin(theta)+cp.y*-Cos(theta)), (v.x-f/m) + (omega-f*t/I)*t = -b*(v.x + omega*t), f*(-1/m-t^2/I)=(-b-1)*(v.x+omega*t)
		//		{
		//			var f = (-bounce - 1) * (rb.velocity.X + rb.omega * t) / (-1.0 / rb.mass - t * t / rb.momentOfInertia);
		//			//f += rb.force.X * secs;
		//			rb.velocity.X -= f / rb.mass;
		//			rb.omega -= f * t / rb.momentOfInertia;
		//		}
		//		t = cp.Y * -Math.Sin(rb.theta) + cp.X * Math.Cos(rb.theta);
		//		if (y == cur_y - 1 && !Blocks.IsCollidable(x, cur_y) && rb.velocity.Y + t * rb.omega < 0)//collide down, +y force. t=(cp.y*-Sin(theta)+cp.x*Cos(theta)), (v.y+f/m) + (omega+f*t/I)*t = -b*(v.y + omega*t), f*(1/m+t^2/I)=(-b-1)*(v.y+omega*t)
		//		{
		//			var f = (-bounce - 1) * (rb.velocity.Y + rb.omega * t) / (1.0 / rb.mass + t * t / rb.momentOfInertia);
		//			//f -= rb.force.Y * secs;
		//			rb.velocity.Y += f / rb.mass;
		//			rb.omega += f * t / rb.momentOfInertia;
		//		}
		//		t = cp.Y * -Math.Sin(rb.theta) + cp.X * Math.Cos(rb.theta);
		//		if (y == cur_y + 1 && !Blocks.IsCollidable(x, cur_y) && rb.velocity.Y + t * rb.omega > 0)//collide up, -y force. t=(cp.y*-Sin(theta)+cp.x*Cos(theta)), (v.y-f/m) + (omega-f*t/I)*t = -b*(v.y + omega*t), f*(-1/m-t^2/I)=(-b-1)*(v.y+omega*t)
		//		{
		//			var f = (-bounce - 1) * (rb.velocity.Y + rb.omega * t) / (-1.0 / rb.mass - t * t / rb.momentOfInertia);
		//			//f += rb.force.Y * secs;
		//			rb.velocity.Y -= f / rb.mass;
		//			rb.omega -= f * t / rb.momentOfInertia;
		//		}
		//		//System.Diagnostics.Trace.WriteLine($"position: {rb.position}, \tvelocity: {rb.velocity}, \ttheta: {rb.theta}, \tomega: {rb.omega}");
		//	}
		//}
		//if (this.CollidePoints.Any(p = > Blocks.IsCollidable((new Point3D() + p) * MatrixTZ, out x, out y))) return false;
		//{
		//    if (!tracks.UpdateRigidBody(secs, Math.Cos(RotationY), RB.velocity - RB._velocity, RB.theta, out Vector3D reactionForce, out double reactionTorque)) return false;
		//    //RB.force += reactionForce;
		//    //RB.alpha += reactionTorque;
		//}
		return false;
	}
	void PodBody::AdvanceRigidBody(const double secs) {
		rb.Advance(secs);
		if (IsRigidBodyMoveTooMuch(secs)) {
			rb.Restore();
			AdvanceRigidBody(0.5 * secs);
			AdvanceRigidBody(0.5 * secs);
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
		propeller(new PodPropeller(pod, PodPropeller::Types::Basic, matrix::TranslateD(glm::dvec3(0, 1.5, 0))* matrix::RotateD(glm::dvec3(1, 0, 0), -PI / 2))){
		rb.mass = 0.8;
		//rb.theta = 0.2;
		children.push_back(new VisibleObject(GetTriangles()));
		children.push_back(propeller);
	}
}