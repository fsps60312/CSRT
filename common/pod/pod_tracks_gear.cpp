#include<common/pod/pod_tracks.hpp>
namespace pod {
	std::vector<Triangle>PodTracks::Track::Gear::GetTriangles(const double radius)const {
		const int n = 10;
		std::vector<glm::dvec3> positions;
		for (int i = 0; i < n; i++) positions.push_back(glm::dvec3(radius * std::cos(2 * PI * i / n), radius * std::sin(2 * PI * i / n), 0));
		std::vector<Triangle>ret;
		for (int i = 2; i < n; i++) {
			ret.push_back(Triangle(glm::mat3(positions[0], positions[i - 1LL], positions[i])));
			ret.push_back(Triangle(glm::mat3(positions[i], positions[i - 1LL], positions[0])));
		}
		return ret;
	}
	PodTracks::Track::Gear::Gear(const glm::dvec3& desired_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod) :
		desired_position(desired_position),
		radius(radius),
		suspension_hardness(suspension_hardness),
		mass(mass),
		pod(pod),
		VisibleObject(GetTriangles(radius)) {
		rb = RigidBody();
		rb.mass = mass;
		rb.position = desired_position;
		transform = matrix::TranslateD(desired_position);
	}
}