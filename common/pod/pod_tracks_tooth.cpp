#include<common/pod/pod_tracks.hpp>
namespace pod {
	std::vector<Triangle> PodTracks::Track::Tooth::GetTriangles()const {
		return Triangle::Cube(glm::vec3(0.1));
	}
	glm::vec3 PodTracks::Track::Tooth::GetPosition()const {
		return position;
	}
	void PodTracks::Track::Tooth::SetPosition(const glm::vec3& position) {
		this->position = position;
		this->transform = matrix::TranslateD(position);
	}
	PodTracks::Track::Tooth::Tooth(const glm::vec3& position) :VisibleObject(GetTriangles()) {
		SetPosition(position);
	}
}