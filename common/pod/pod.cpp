#include<common/pod/pod.hpp>
namespace pod {
	Pod::Pod() :PodInterface(),
		body(new PodBody(this)),
		tracks(new PodTracks(this, glm::dvec3(0, -1.5, 0))) {
		this->children.insert(body);
		this->children.insert(tracks);
	}
	bool Pod::IsOnGround()const {
		return tracks->IsOnGround();
		//return false;
	}
	bool Pod::IsPodStableOnGround()const {
		if (!IsOnGround())return false;
		const glm::dvec3 total_force = body->GetForce() + tracks->GetTotalReactForce();
		if (glm::length(total_force) > 1)return false;
		if (glm::length(body->GetRigidBody()->velocity) > 1)return false;
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
	double Pod::GetCorrectiveAlpha()const {
		return body->GetCorrectiveAlpha();
	}
	block::Block* Pod::GetCollideFront()const {
		return tracks->CollideFront();
	}
	block::Block* Pod::GetCollideDown()const {
		block::Block* ret;
		block::IsCollidable(body->GetRigidBody()->position + glm::dvec3(0, -constants::block_height, 0), ret);
		return ret;
	}
}