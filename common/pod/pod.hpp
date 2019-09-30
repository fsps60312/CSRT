#pragma once
#include<common/pod/pod_interface.hpp>
#include<common/pod/pod_body.hpp>
namespace pod {
	class Pod :public PodInterface {
	private:
		PodBody* body;
	public:
		Pod();
		bool IsOnGround()const override;
		bool IsPodStopped()const override;
		glm::dmat4 GetMatrixY()const override;
	};
}