#include<common/pod/pod_tracks.hpp>
namespace pod {
	std::vector<Triangle> PodTracks::Track::Tooth::GetTriangles()const {
		Material mtl;
		mtl.diffuse = glm::vec3(0.5f);
		mtl.ambient = mtl.diffuse * 0.5f;
		mtl.alpha = 1;
		return Triangle::Cube(glm::vec3(0.1f), Material::GetMaterialId("pod_track_tooth", mtl));
	}
	glm::vec3 PodTracks::Track::Tooth::GetPosition()const {
		return position;
	}
	void PodTracks::Track::Tooth::SetPosition(const glm::vec3& position) {
		this->position = position;
		SetTransform(matrix::TranslateD(position));
	}
	PodTracks::Track::Tooth::Tooth(const glm::vec3& position) :VisibleObject(GetTriangles()) {
		SetPosition(position);
	}
}