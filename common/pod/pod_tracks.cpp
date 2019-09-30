#include<common/pod/pod_tracks.hpp>
namespace pod {
	PodTracks::PodTracks(PodInterface* pod, const glm::dvec3& offset){
		const double gap = 2.5 + 1;
		left_track = new Track(pod, offset + glm::dvec3(0, 0, -gap / 2));
		rigt_track = new Track(pod, offset + glm::dvec3(0, 0,  gap / 2));
		//leftTrack.Transform = leftTrack.OriginTransform = MyLib.Transform(leftTrack).TranslatePrepend(new Vector3D(0, 0, -gap / 2)).Value;
		//rigtTrack.Transform = rigtTrack.OriginTransform = MyLib.Transform(rigtTrack).TranslatePrepend(new Vector3D(0, 0, gap / 2)).Value;
		children.push_back(left_track);
		children.push_back(rigt_track);
	}
	PodTracks::Track::Track(PodInterface* pod, const glm::dvec3& offset) :
		pod(pod) {
		const double depth = 1, height = 1, lengthUp = 4, lengthDown = 2.5;
		gears = {
			//                  relative position,                                                      radius, suspension hardness,  mass
			new Gear(offset + glm::dvec3(-lengthUp / 2, -height / 2 + 0.4, 0),                           0.4,                 7.0,  0.02, pod),
			new Gear(offset + glm::dvec3((lengthDown * 1 - lengthUp * 3) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                10.0, 0.025, pod),
			new Gear(offset + glm::dvec3((lengthDown * 2 - lengthUp * 2) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                10.0,  0.03, pod),
			new Gear(offset + glm::dvec3((lengthDown * 3 - lengthUp * 1) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                20.0,  0.03, pod),
			new Gear(offset + glm::dvec3(lengthDown / 2, -height / 2 + 0.5, 0),                          0.5,                15.0, 0.045, pod),
			new Gear(offset + glm::dvec3(lengthUp / 2, height / 2, 0),                                   0.2,                10.0,  0.01, pod),
			new Gear(offset + glm::dvec3(-lengthUp / 10, height / 2, 0),                                 0.3,                10.0,  0.01, pod),
		};
		for (Gear* gv : gears) children.push_back(gv);
		for (const int i : {3, 4})ground_gears.push_back(gears[i]);
		const double chainLength = GetChainLength(gears);
		const int count = (int)(chainLength / 0.5);
		for (int i = 0; i < count; i++) {
			auto t = new Tooth(GetToothPosition(gears, (double)i / count));
			children.push_back(t);
			teeth.push_back(t);
		}
	}
	glm::vec3 PodTracks::Track::GetToothPosition(const std::vector<Gear*>& chain, const double ratio) {
		double target = GetChainLength(chain) * ratio;
		double ans = 0;
		for (int i = 0; i < (int)chain.size(); i++) {
			const auto &a = chain[i];
			const auto &b = chain[(i + 1) % (int)chain.size()];
			const auto &c = chain[(i + 2) % (int)chain.size()];
			const auto &t1 = GetChainTouchPoint(a, b);
			const auto &t2 = GetChainTouchPoint(b, c);
			double l;
			l = std::sqrt(std::pow(glm::length(b->desired_position - a->desired_position), 2) + std::pow(b->radius - a->radius, 2));
			if (ans + l <= target) ans += l;
			else
			{
				const double r = (target - ans) / l;
				return (1 - r) * (a->desired_position + t1 * a->radius) + r * (b->desired_position + t1 * b->radius);
			}
			const double angle = matrix::AngleBetween(t1, t2);
			l = angle * b->radius;
			if (ans + l <= target) ans += l;
			else
			{
				const double r = (target - ans) / l;
				const glm::dmat4& mat = matrix::RotateD(matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)), angle * r);
				return b->desired_position + matrix::Multiply(mat, t1) * b->radius;
			}
		}
		throw;
	}
	glm::dvec3 PodTracks::Track::GetChainTouchPoint(Gear* a, Gear* b)const {
		const double angle = PI / 2 - std::acos((a->radius - b->radius) / glm::length(a->desired_position - b->desired_position));
		const glm::dmat4& mat = matrix::RotateD(matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)), angle);
		const glm::dvec3& ans = glm::normalize(matrix::Multiply(mat, glm::cross(b->desired_position - a->desired_position, matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)))));
		return ans;
	}
	double PodTracks::Track::GetChainLength(const std::vector<Gear*>& chain)const {
		double ans = 0;
		for (int i = 0; i < (int)chain.size(); i++) {
			const auto &a = chain[i];
			const auto &b = chain[(i + 1LL) % chain.size()];
			const auto &c = chain[(i + 2LL) % chain.size()];
			ans += std::sqrt(std::pow(glm::length(b->desired_position - a->desired_position), 2) + std::pow(b->radius - a->radius, 2));
			ans += matrix::AngleBetween(GetChainTouchPoint(a, b), GetChainTouchPoint(b, c)) * b->radius;
		}
		return ans;
	}
}