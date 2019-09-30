#include<common/pod/pod_tracks.hpp>
namespace pod {
	PodTracks::Track::Gear::Gear(const glm::dvec3& desired_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod) :
		desired_position(desired_position),
		radius(radius),
		suspension_hardness(suspension_hardness),
		mass(mass),
		pod(pod) {
	}
}