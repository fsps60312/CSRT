#pragma once
#include<common/pod/pod_interface.hpp>
#include<common/pod/pod_body.hpp>
#include<common/pod/pod_tracks.hpp>
namespace pod {
	class Pod :public PodInterface {
	private:
		PodBody* body;
		PodTracks* tracks;
	public:
		bool IsOnGround()const override;
		bool IsPodStableOnGround()const override;
		glm::dmat4 GetMatrixY()const override;
		glm::dmat4 GetMatrixZ()const override;
		glm::dmat4 GetMatrixT()const override;
		RigidBody* GetRigidBody() override;
		double GetCorrectiveAlpha()const override;
		block::Block* GetCollideFront()const override;
		block::Block* GetCollideDown()const override;
		Pod();
	};
}