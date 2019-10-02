#pragma once
#include<common/pod/pod_interface.hpp>
#include<common/pod/pod_body.hpp>
#include<common/pod/pod_tracks.hpp>
#include<common/pod/pod_propeller.hpp>
namespace pod {
	class Pod :public PodInterface {
	private:
		PodBody* body;
		PodTracks* tracks;
		PodPropeller* propeller;
	public:
		Pod();
		bool IsOnGround()const override;
		bool IsPodStopped()const override;
		glm::dmat4 GetMatrixY()const override;
		glm::dmat4 GetMatrixZ()const override;
		glm::dmat4 GetMatrixT()const override;
		RigidBody* GetRigidBody() override;
	};
}