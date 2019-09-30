#include<common/pod/pod_tracks.hpp>
namespace pod {
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