#include<common/pod/pod_body.hpp>
namespace pod {
	void PodBody::MaintainRotationZ(const double secs) {
		//TODO
	}
	void PodBody::MaintainRotationY(const double secs) {
		//TODO
	}
	void PodBody::MaintainRigidBody(const double secs) {
		//TODO
	}
	void PodBody::Advance(const double secs) {
		MaintainRotationY(secs);
		MaintainRotationZ(secs);
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
	PodBody::PodBody() :VisibleObject(GetTriangles()) {
		rb.mass = 0.8;
	}
}