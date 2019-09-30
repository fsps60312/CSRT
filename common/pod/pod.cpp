#include<common/pod/pod.hpp>
namespace pod {
	Pod::Pod() :PodInterface(),
		body(new PodBody(this)) {
		this->children.push_back((VisibleObject*)body);
	}
	bool Pod::IsOnGround()const {
		return false;
	}
	bool Pod::IsPodStopped()const {
		return true;
	}
	glm::dmat4 Pod::GetMatrixY()const {
		return body->GetMatrixY();
	}
}