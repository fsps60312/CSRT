#include<common/pod/pod_drill.hpp>
namespace pod {
	void PodDrill::Update(const double secs) {
		const double fold_speed = 2.0;
		if (fold_target > fold_state) {
			SetFoldState(std::min(fold_target, fold_state + fold_speed * secs));
		} else {
			SetFoldState(std::max(fold_target, fold_state - fold_speed * secs));
		}
	}
	void PodDrill::SetFoldTarget(const double fold_target) {
		this->fold_target = fold_target;
	}
	double PodDrill::GetFoldState()const {
		return fold_state;
	}
	void PodDrill::SetFoldState(const double fold_state) {
		this->fold_state = fold_state;
		const double ratio = 0.3;
		const glm::dmat4& dir_transform = fold_state > 0 ? matrix::IdentityD() : matrix::RotateD(glm::dvec3(0, 0, 1), -90.0 / 180 * PI);
		if ((1 - fabs(fold_state)) < ratio) {
			double f = (1 - fabs(fold_state)) / ratio;
			for (Blade* blade : blades) blade->SetFoldState(f);
			SetTransform(dir_transform * basic_transform);
		} else {
			double f = ((1 - fabs(fold_state)) - ratio) / (1 - ratio);
			for (Blade* blade : blades) blade->SetFoldState(1);
			SetTransform(dir_transform * basic_transform * matrix::TranslateD(glm::dvec3(0, 0, -f * radius / std::cos(cone_angle))));
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