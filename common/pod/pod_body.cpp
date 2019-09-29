#include<common/pod/pod_body.hpp>
namespace pod {
	void PodBody::MaintainRotationZ(const double secs) {
		const double t = -10 * rb.theta - rb.omega;
		rb.alpha = std::pow(std::abs(t), 1) * t;
		if (environment::IsKeyDown(GLFW_KEY_A))rb.alpha += 5;
		if (environment::IsKeyDown(GLFW_KEY_D))rb.alpha -= 5;
		rb.alpha += -1 * std::abs(rb.omega) * rb.omega - 1 * rb.omega;
	}
	void PodBody::MaintainRotationY(const double secs) {
		if ((!parent->IsOnGround() || parent->IsPodStopped())) {
			if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D))desired_rotation_y = PI;
			if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A))desired_rotation_y = 0;
		}
		mylib::SmoothTo(rotation_y, desired_rotation_y, secs, std::max(std::abs(desired_rotation_y - rotation_y) / PI, 0.2) * 0.2);
	}
	void PodBody::MaintainRigidBody(const double secs) {
		//TODO
	}
	void PodBody::Advance(const double secs) {
		MaintainRotationY(secs);
		MaintainRotationZ(secs);
		transform = matrix::RotateD(glm::dvec3(0, -1, 0), rotation_y) * matrix::RotateD(glm::dvec3(0, 0, 1), rb.theta);
		{
			rb.position.z = rb.velocity.z = 0;
			rb.force = glm::dvec3();
			rb.force += glm::dvec3(0, -rb.mass * constants::gravity, 0);
			//rb.force += new glm::dvec3(-sin(rb.theta) * propeller.LiftForce(), cos(rb.theta) * propeller.LiftForce(), 0);
			MaintainRigidBody(secs);
		}
		{
			// TODO
		}
	}
	const double PodBody::body_radius = 1.5;
	std::vector<glm::mat3> PodBody::GetTriangles() {
		glm::vec3 ps[8] = {
			glm::vec3(-body_radius,-body_radius, body_radius),
			glm::vec3( body_radius,-body_radius, body_radius),
			glm::vec3(-body_radius,-body_radius,-body_radius),
			glm::vec3( body_radius,-body_radius,-body_radius),
			glm::vec3(-body_radius, body_radius, body_radius),
			glm::vec3( body_radius, body_radius, body_radius),
			glm::vec3(-body_radius, body_radius,-body_radius),
			glm::vec3( body_radius, body_radius,-body_radius),
		};
		/*
		  o----o
		 /    /|
		o----o |
		|    | o
		|    |/
		o----o
		*/
		return {
			glm::mat3(ps[0],ps[2],ps[3]),
			glm::mat3(ps[0],ps[3],ps[1]),
			glm::mat3(ps[0],ps[1],ps[5]),
			glm::mat3(ps[0],ps[5],ps[4]),
			glm::mat3(ps[0],ps[4],ps[6]),
			glm::mat3(ps[0],ps[6],ps[2]),
			glm::mat3(ps[7],ps[6],ps[4]),
			glm::mat3(ps[7],ps[4],ps[5]),
			glm::mat3(ps[7],ps[5],ps[1]),
			glm::mat3(ps[7],ps[1],ps[3]),
			glm::mat3(ps[7],ps[3],ps[2]),
			glm::mat3(ps[7],ps[2],ps[6])
		};
	}
	PodBody::PodBody(PodInterface* parent) :VisibleObject(GetTriangles()),
		parent(parent) {
		rb.mass = 0.8;
	}
}