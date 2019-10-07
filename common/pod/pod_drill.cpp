#include<common/pod/pod_drill.hpp>
namespace pod {
	void PodDrill::Update(const double secs) {
		const double fold_period = 0.5;
		if (pod->IsOnGround()) {
			const double nxt_fold_state = std::max(0.0, fold_state - secs / fold_period);
			SetFoldState(nxt_fold_state);
		} else {
			const double nxt_fold_state = std::min(1.0, fold_state + secs / fold_period);
			SetFoldState(nxt_fold_state);
		}
	}
	void PodDrill::SetFoldState(const double fold_state) {
		this->fold_state = fold_state;
		const double ratio = 0.3;
		if (fold_state < ratio) {
			double f = fold_state / ratio;
			for (Blade* blade : blades) blade->SetFoldState(f);
			SetTransform(basic_transform);
		} else {
			double f = (fold_state - ratio) / (1 - ratio);
			for (Blade* blade : blades) blade->SetFoldState(1);
			SetTransform(basic_transform * matrix::TranslateD(glm::dvec3(0, 0, -f * radius / std::cos(cone_angle))));
		}
	}
	PodDrill::PodDrill(PodInterface* pod, const double radius, const int blade_count, const glm::dmat4& basic_transform) :
		pod(pod), radius(radius), cone_angle(50.0 / 180 * PI), basic_transform(basic_transform) {
		for (int i = 0; i < blade_count; i++) {
			double a = 2.0 * PI * i / blade_count;
			Blade* blade = new Blade(radius, a, cone_angle, 40.0 / 180 * PI);
			blades.push_back(blade);
			children.insert(blade);
		}
		SetFoldState(0);
	}
}