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
		Pod();
		bool IsOnGround()const override;
		bool IsPodStopped()const override;
		glm::dmat4 GetMatrixY()const override;
	};
}