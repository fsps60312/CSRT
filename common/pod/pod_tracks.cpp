#include<common/pod/pod_tracks.hpp>
namespace pod {
	glm::dvec3 PodTracks::GetTotalReactForce()const {
		return left_track->GetTotalReactForce() + rigt_track->GetTotalReactForce();
	}
	glm::dvec3 PodTracks::Track::GetTotalReactForce()const {
		glm::dvec3 ret(0);
		for (Gear* gear : gears)ret += gear->GetReactForce();
		return ret;
	}
	void PodTracks::Track::UpdateTrackSpeed(const double secs) {
		const double deacceleration = environment::IsKeyDown(GLFW_KEY_A) && environment::IsKeyDown(GLFW_KEY_D) ? 100 : 20;
		double track_cycle_acceleration = track_cycle_speed > 0 ? -deacceleration : deacceleration;
		if (pod->IsOnGround()) {
			if (environment::IsKeyDown(GLFW_KEY_A) && !environment::IsKeyDown(GLFW_KEY_D))track_cycle_acceleration += 70;
			if (environment::IsKeyDown(GLFW_KEY_D) && !environment::IsKeyDown(GLFW_KEY_A))track_cycle_acceleration += 70;
		}
		track_cycle_speed += track_cycle_acceleration * secs;
	}
	void PodTracks::Track::AddTrackSpeed(const double dv) {
		track_cycle_speed += dv;
	}
	double PodTracks::Track::GetTrackSpeed()const {
		return track_cycle_speed;
	}
	bool PodTracks::Track::IsOnGround() const{
		for (Gear* gear : ground_gears)if (!gear->IsOnGround())return false;
		return true;
	}
	void PodTracks::Track::Advance(const double secs) {
		UpdateTrackSpeed(secs);
		for (auto& ch : children)ch->Advance(secs); // render first so teeth will be at correct position
		const double chainLength = GetChainLength(gears);
		track_cycle_position -= secs * track_cycle_speed / chainLength;
		track_cycle_position = std::fmod(std::fmod(track_cycle_position, 1) + 1, 1);
		//std::clog << "hi" << std::endl;
		for (int i = 0; i < (int)teeth.size(); i++) {
			const glm::dvec3& position = GetToothPosition(gears, fmod((double)i / (int)teeth.size() + track_cycle_position, 1));
			//std::clog << "\t" << position.x << "\t" << position.y << "\t" << position.z << std::endl;
			teeth[i]->SetPosition(position);
		}
	}
	PodTracks::Track::Track(PodInterface* pod, const glm::dvec3& offset) :
		pod(pod) {
		const double depth = 1, height = 1, lengthUp = 4, lengthDown = 2.5;
		gears = {
			//                  relative position,                                                      radius, suspension hardness,  mass
			new Gear(offset + glm::dvec3(                - lengthUp / 2,          -height / 2 + 0.4, 0), 0.4,                 7.0,  0.02, pod, this),
			new Gear(offset + glm::dvec3((lengthDown * 1 - lengthUp * 3) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                10.0, 0.025, pod, this),
			new Gear(offset + glm::dvec3((lengthDown * 2 - lengthUp * 2) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                10.0,  0.03, pod, this),
			new Gear(offset + glm::dvec3((lengthDown * 3 - lengthUp * 1) / 4 / 2, -height / 2 + 0.2, 0), 0.2,                20.0,  0.03, pod, this),
			new Gear(offset + glm::dvec3( lengthDown / 2,                         -height / 2 + 0.5, 0), 0.5,                15.0, 0.045, pod, this),
			new Gear(offset + glm::dvec3(                  lengthUp / 2,           height / 2 + 0.0, 0), 0.2,                10.0,  0.01, pod, this),
			new Gear(offset + glm::dvec3(                - lengthUp / 10,          height / 2 + 0.0, 0), 0.3,                10.0,  0.01, pod, this),
		};
		for (Gear* gv : gears) children.insert(gv);
		for (const int i : {0, 1, 2, 3, 4})ground_gears.push_back(gears[i]);
		const double chain_length = GetChainLength(gears);
		std::clog << "chain length = " << chain_length << std::endl;
		const int count = (int)(chain_length / 0.3);
		for (int i = 0; i < count; i++) {
			auto t = new Tooth(GetToothPosition(gears, (double)i / count));
			children.insert(t);
			teeth.push_back(t);
		}
	}
	glm::vec3 PodTracks::Track::GetToothPosition(const std::vector<Gear*>& chain, const double ratio) {
		double target_position = GetChainLength(chain) * ratio;
		double current_position = 0;
		for (int i = 0; i < (int)chain.size(); i++) {
			const auto &a = chain[i];
			const auto &b = chain[(i + 1) % (int)chain.size()];
			const auto &c = chain[(i + 2) % (int)chain.size()];
			const auto &t1 = GetGearToSubchainVector(a, b);
			const auto &t2 = GetGearToSubchainVector(b, c);
			const double t = std::pow(glm::length(b->GetPosition() - a->GetPosition()), 2) - std::pow(b->radius - a->radius, 2);
			if (t > 0) {
				const double l = std::sqrt(t);
				if (current_position + l <= target_position) current_position += l;
				else {
					const double r = (target_position - current_position) / l;
					return (1 - r) * (a->GetPosition() + t1 * a->radius) + r * (b->GetPosition() + t1 * b->radius);
				}
			}
			if (t1 != glm::dvec3(0) && t2 != glm::dvec3(0)) {
				const double angle = matrix::AngleBetween(t1, t2);
				const double l = angle * b->radius;
				if (current_position + l <= target_position) current_position += l;
				else {
					const double r = (target_position - current_position) / l;
					const glm::dmat4& mat = matrix::RotateD(matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)), angle * r);
					return b->GetPosition() + matrix::Multiply(mat, t1) * b->radius;
				}
			}
		}
		throw;
	}
	glm::dvec3 PodTracks::Track::GetGearToSubchainVector(Gear* a, Gear* b)const {
		const double cos_value = (a->radius - b->radius) / glm::distance(a->GetPosition(), b->GetPosition());
		if (cos_value < -1 || 1 < cos_value)return glm::dvec3(0);
		const double angle = PI / 2 - std::acos(cos_value);
		const glm::dmat4& mat = matrix::RotateD(matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)), angle);
		const glm::dvec3& ans = glm::normalize(matrix::Multiply(mat, glm::cross(b->GetPosition() - a->GetPosition(), matrix::Multiply(pod->GetMatrixY(), glm::dvec3(0, 0, 1)))));
		return ans;
	}
	double PodTracks::Track::GetChainLength(const std::vector<Gear*>& chain)const {
		double ans = 0;
		for (int i = 0; i < (int)chain.size(); i++) {
			const auto &a = chain[i];
			const auto &b = chain[(i + 1LL) % chain.size()];
			const auto &c = chain[(i + 2LL) % chain.size()];
			const auto& t1 = GetGearToSubchainVector(a, b);
			const auto& t2 = GetGearToSubchainVector(b, c);
			const double t = std::pow(glm::length(b->GetPosition() - a->GetPosition()), 2) - std::pow(b->radius - a->radius, 2);
			if (t > 0)ans += std::sqrt(t);
			if (t1 != glm::dvec3(0) && t2 != glm::dvec3(0))ans += matrix::AngleBetween(t1, t2) * b->radius;
		}
		return ans;
	}
	bool PodTracks::IsOnGround()const {
		return left_track->IsOnGround() && rigt_track->IsOnGround();
	}
	PodTracks::PodTracks(PodInterface* pod, const glm::dvec3& offset) {
		const double gap = 2.5 + 1;
		left_track = new Track(pod, offset + glm::dvec3(0, 0, -gap / 2));
		rigt_track = new Track(pod, offset + glm::dvec3(0, 0, gap / 2));
		//leftTrack.Transform = leftTrack.OriginTransform = MyLib.Transform(leftTrack).TranslatePrepend(new Vector3D(0, 0, -gap / 2)).Value;
		//rigtTrack.Transform = rigtTrack.OriginTransform = MyLib.Transform(rigtTrack).TranslatePrepend(new Vector3D(0, 0, gap / 2)).Value;
		children.insert(left_track);
		children.insert(rigt_track);
	}
}