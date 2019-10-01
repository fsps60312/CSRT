#include<common/pod/pod_tracks.hpp>
namespace pod {
	bool PodTracks::Track::Gear::IsOnGround() const{
		return on_ground_countdown > 0;
	}
	void PodTracks::Track::Gear::RotateYAlongWithPod() {
		const glm::dmat4& mat_y = pod->GetMatrixY(), & mat_z = pod->GetMatrixZ(), & mat_t = pod->GetMatrixT();
		const glm::dmat4& mat_trans = matrix::Inverse(pre_matrix_y * mat_z * mat_t) * mat_y * mat_z * mat_t;
		const glm::dvec3& new_position = matrix::Multiply(mat_trans, rb.position);
		const glm::dvec3& new_velocity = matrix::Multiply(mat_trans, rb.velocity);
		//RB.position.X = newPosition.X; RB.position.Z = newPosition.Z;
		rb.position = new_position;
		rb.velocity = new_velocity;
		//RB.velocity.X = newVelocity.X; RB.velocity.Y = newVelocity.Y;
		pre_matrix_y = mat_y;
	}
	void PodTracks::Track::Gear::ApplyReactForceWithPod() {
		const glm::dvec3& react_force = GetReactForce();
		pod->GetRigidBody()->force += react_force;
		rb.force -= react_force;
	}
	void PodTracks::Track::Gear::Update() {
		RotateYAlongWithPod();
		ApplyReactForceWithPod();
	}
	void PodTracks::Track::Gear::AdvanceRigidBody(const double secs) {
		if (!rb.Advance(secs,[secs,this](RigidBody* rb) ->bool
		{
			//if (secs > 1e-3) return false;
			const double dif = glm::length(rb->position - rb->_position);
			if (dif > radius / 5) return false;
			//if (!Blocks.IsCollidable(rb.position, out int cur_x, out int cur_y))
			//{
			//	//bool rollback = false;
			//	const double rollbackSpeed = 0.1;
			//	const double bounce = 0.1;
			//	{
			//		int x = 0, y = 0;
			//		Point3D cp;
			//		cp = rb.position + new Vector3D(-Radius, 0, 0);
			//		if (Blocks.IsCollidable(cp, out x, out y))
			//		{
			//			if (x == cur_x - 1 && !Blocks.IsCollidable(cur_x, y) && rb.velocity.X < 0)//collide left, +x force. t=(cp.x*-Sin(theta)+cp.y*-Cos(theta)), (v.x+f/m) + (omega+f*t/I)*t = -b*(v.x + omega*t), f*(1/m+t^2/I)=(-b-1)*(v.x+omega*t)
			//			{
			//				var f = (-bounce - 1) * (rb.velocity.X) / (1.0 / rb.mass);
			//				rb.velocity.X += f / rb.mass;
			//				//rollback = true;
			//				rb.position.X += secs * rollbackSpeed;
			//			}
			//		}
			//		cp = rb.position + new Vector3D(Radius, 0, 0);
			//		if (Blocks.IsCollidable(cp, out x, out y))
			//		{
			//			if (x == cur_x + 1 && !Blocks.IsCollidable(cur_x, y) && rb.velocity.X > 0)//collide right, -x force. t=(cp.x*-Sin(theta)+cp.y*-Cos(theta)), (v.x-f/m) + (omega-f*t/I)*t = -b*(v.x + omega*t), f*(-1/m-t^2/I)=(-b-1)*(v.x+omega*t)
			//			{
			//				var f = (-bounce - 1) * (rb.velocity.X) / (-1.0 / rb.mass);
			//				rb.velocity.X -= f / rb.mass;
			//				//rollback = true;
			//				rb.position.X -= secs * rollbackSpeed;
			//			}
			//		}
			//		cp = rb.position + new Vector3D(0, -Radius, 0);
			//		if (Blocks.IsCollidable(cp, out x, out y))
			//		{
			//			if (y == cur_y - 1 && !Blocks.IsCollidable(x, cur_y) && rb.velocity.Y < 0)//collide down, +y force. t=(cp.y*-Sin(theta)+cp.x*Cos(theta)), (v.y+f/m) + (omega+f*t/I)*t = -b*(v.y + omega*t), f*(1/m+t^2/I)=(-b-1)*(v.y+omega*t)
			//			{
			//				var f = (-bounce - 1) * (rb.velocity.Y) / (1.0 / rb.mass);
			//				rb.velocity.Y += f / rb.mass;
			//				//rollback = true;
			//				rb.position.Y += secs * rollbackSpeed;
			//				onGround = 0.1;
			//			}
			//		}
			//		cp = rb.position + new Vector3D(0, Radius, 0);
			//		if (Blocks.IsCollidable(cp, out x, out y))
			//		{
			//			if (y == cur_y + 1 && !Blocks.IsCollidable(x, cur_y) && rb.velocity.Y > 0)//collide up, -y force. t=(cp.y*-Sin(theta)+cp.x*Cos(theta)), (v.y-f/m) + (omega-f*t/I)*t = -b*(v.y + omega*t), f*(-1/m-t^2/I)=(-b-1)*(v.y+omega*t)
			//			{
			//				var f = (-bounce - 1) * (rb.velocity.Y) / (-1.0 / rb.mass);
			//				rb.velocity.Y -= f / rb.mass;
			//				//rollback = true;
			//				rb.position.Y -= secs * rollbackSpeed;
			//			}
			//		}
			//	}
			//	//if(false)
			//	{
			//		Blocks.IsCollidable(rb.position + new Vector3D(Blocks.Width / 2, Blocks.Height / 2, 0), out int cross_x, out int cross_y);
			//		var vectorToCross = new Vector3D((Blocks.Anchor.X + Blocks.Width * cross_x) - rb.position.X, (Blocks.Anchor.Y + Blocks.Height * cross_y) - rb.position.Y, 0);
			//		if (0 < vectorToCross.Length && vectorToCross.Length < Radius && Vector3D.DotProduct(rb.velocity, vectorToCross) > 0)
			//		{
			//			int x = vectorToCross.X < 0 ? cross_x - 1 : cross_x;
			//			int y = vectorToCross.Y < 0 ? cross_y - 1 : cross_y;
			//			if (Blocks.IsCollidable(x, y))
			//			{
			//				///(v+a*c)。c=-b*(v。c)
			//				///(vx+a*cx)*cx+(vy+a*cy)*cy=-b*(vx*cx+vy*cy)
			//				///a(cx*cx+cy*cy)+vx*cx+vy*cy=-b*(vx*cx+vy*cy)
			//				///a=(-b*(vx*cx+vy*cy)-(vx*cx+vy*cy))/(cx*cx+cy*cy)
			//				double a = ((-bounce - 1) * Vector3D.DotProduct(rb.velocity, vectorToCross)) / Vector3D.DotProduct(vectorToCross, vectorToCross);
			//				rb.velocity += a * vectorToCross;
			//				//rollback = true;
			//				rb.position += -vectorToCross / vectorToCross.Length * secs * rollbackSpeed;
			//			}
			//		}
			//	}
			//	//if (rollback) rb.position = rb._position;
			//	//System.Diagnostics.Trace.WriteLine($"position: {rb.position}, \tvelocity: {rb.velocity}, \ttheta: {rb.theta}, \tomega: {rb.omega}");
			//}
			return true;
		}))
		{
			AdvanceRigidBody(0.5 * secs);
			AdvanceRigidBody(0.5 * secs);
		};
	}
	void PodTracks::Track::Gear::Advance(const double secs) {
		AdvanceRigidBody(secs);
		transform = matrix::TranslateD(rb.position) * pod->GetMatrixY() * matrix::RotateD(glm::dvec3(0, 0, -1), rb.theta);
	}
	glm::dvec3 PodTracks::Track::Gear::GetReactForce()const {
		const glm::dvec3 on_pod_velocity = pod->GetRigidBody()->GetVelocityAt(matrix::Multiply(pod->GetMatrixY(), relative_position));
		const glm::dvec3&
			elastic_force = suspension_hardness * (GetPosition() - GetDesiredPosition()),
			damp_force = 0.5 * (rb.velocity - on_pod_velocity);
		return elastic_force + damp_force;
	}
	glm::dvec3 PodTracks::Track::Gear::GetDesiredPosition()const {
		return  matrix::Multiply(pod->GetMatrixY() * pod->GetMatrixZ() * pod->GetMatrixT(), relative_position);
	}
	glm::dvec3 PodTracks::Track::Gear::GetPosition()const {
		return rb.position;
	}
	std::vector<Triangle>PodTracks::Track::Gear::GetTriangles(const double radius)const {
		const int n = 10;
		std::vector<glm::dvec3> positions;
		for (int i = 0; i < n; i++) positions.push_back(glm::dvec3(radius * std::cos(2 * PI * i / n), radius * std::sin(2 * PI * i / n), 0));
		std::vector<Triangle>ret;
		for (int i = 2; i < n; i++) {
			ret.push_back(Triangle(glm::mat3(positions[0], positions[i - 1LL], positions[i])));
			ret.push_back(Triangle(glm::mat3(positions[i], positions[i - 1LL], positions[0])));
		}
		return ret;
	}
	PodTracks::Track::Gear::Gear(const glm::dvec3& relative_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod) :
		relative_position(relative_position),
		radius(radius),
		suspension_hardness(suspension_hardness),
		mass(mass),
		pod(pod),
		pre_matrix_y(pod->GetMatrixY()),
		VisibleObject(GetTriangles(radius)) {
		rb = RigidBody();
		rb.mass = mass;
		rb.position = GetDesiredPosition();
		transform = matrix::TranslateD(relative_position);
	}
}