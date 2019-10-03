#include<common/pod/pod.hpp>
namespace pod {
	Pod::Pod() :PodInterface(),
		body(new PodBody(this)),
		tracks(new PodTracks(this, glm::dvec3(0, -1.5, 0))) {
		this->children.push_back(body);
		this->children.push_back(tracks);
	}
	bool Pod::IsOnGround()const {
		return tracks->IsOnGround();
		//return false;
	}
	bool Pod::IsPodStopped()const {
		return true;
	}
	glm::dmat4 Pod::GetMatrixY()const {
		return body->GetMatrixY();
	}
	glm::dmat4 Pod::GetMatrixZ()const {
		return body->GetMatrixZ();
	}
	glm::dmat4 Pod::GetMatrixT()const {
		return body->GetMatrixT();
	}
	RigidBody* Pod::GetRigidBody() {
		return body->GetRigidBody();
	}
}