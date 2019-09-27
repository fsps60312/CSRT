#include<common/pod/pod_body.hpp>
namespace pod {
	std::vector<glm::mat3> PodBody::GetTriangles() {
		glm::vec3 ps[8] = {
			glm::vec3(-1,-1, 1),
			glm::vec3( 1,-1, 1),
			glm::vec3(-1,-1,-1),
			glm::vec3( 1,-1,-1),
			glm::vec3(-1, 1, 1),
			glm::vec3( 1, 1, 1),
			glm::vec3(-1, 1,-1),
			glm::vec3( 1, 1,-1),
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
	}
}