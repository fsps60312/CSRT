#include<common/pod/pod_body.hpp>
namespace pod {
	glm::dmat4 PodBody::GetMatrixY()const {
		return matrix::RotateD(glm::dvec3(0, -1, 0), rotation_y);
	}
	glm::dmat4 PodBody::GetMatrixZ()const {
		return matrix::RotateD(glm::dvec3(0, 0, 1), rb.theta);
	}
	glm::dmat4 PodBody::GetMatrixT()const {
		return matrix::TranslateD(rb.position);
	}
	RigidBody* PodBody::GetRigidBody() {
		return &rb;
	}
	void PodBody::DesiredRotationZ() {
		const double t = -10 * rb.theta - rb.omega;
		rb.alpha = std::pow(std::abs(t), 1) * t;
		if (environment::IsKeyDown(GLFW_KEY_A))rb.alpha += 5;
		if (environment::IsKeyDown(GLFW_KEY_D))rb.alpha -= 5;
		rb.alpha += -1 * std::abs(rb.omega) * rb.omega - 1 * rb.omega;
	}
	void PodBody::DesiredRotationY() {
		if ((!parent->IsOnGround() || parent->IsPodStopped())) {
			if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D))desired_rotation_y = PI;
			if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A))desired_rotation_y = 0;
		}
	}
	void PodBody::Update() {
		DesiredRotationY();
		DesiredRotationZ();
		{
			rb.position.z = rb.velocity.z = 0;
			rb.force = glm::dvec3();
			rb.force += glm::dvec3(0, -rb.mass * constants::gravity, 0);
			//rb.force += new glm::dvec3(-sin(rb.theta) * propeller.LiftForce(), cos(rb.theta) * propeller.LiftForce(), 0);
		}
	}
	void PodBody::AdvanceRigidBody(const double secs) {
		if (!rb.Advance(secs,[secs](RigidBody* rb) -> bool {
			//if (secs > 1e-3) return false;
			const double dif = glm::length(rb->position - rb->_position);
			if (dif > 0.5) return false;
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
			return true;
		}))
		{
			AdvanceRigidBody(0.5 * secs);
			AdvanceRigidBody(0.5 * secs);
		}
	}
	void PodBody::Advance(const double secs) {
		mylib::SmoothTo(rotation_y, desired_rotation_y, secs, std::max(std::abs(desired_rotation_y - rotation_y) / PI, 0.2) * 0.2);
		AdvanceRigidBody(secs);
		transform = GetMatrixT() * GetMatrixY() * GetMatrixZ();
	}
	const double PodBody::body_radius = 1.5;
	std::vector<Triangle> PodBody::GetTriangles()const {
		return Triangle::Cube(glm::vec3(body_radius));
	}
	PodBody::PodBody(PodInterface* parent) :VisibleObject(GetTriangles()),
		parent(parent) {
		rb.mass = 0.8;
	}
}