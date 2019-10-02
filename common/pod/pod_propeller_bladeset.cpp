#include<common/pod/pod_propeller.hpp>
namespace pod {
	const std::map<PodPropeller::BladeSet::Types, PodPropeller::BladeSet::Description>PodPropeller::BladeSet::descriptions = {
		{Types::Basic,Description(Blade::Types::BasicTriangle,3)}
	};
	PodPropeller::BladeSet::Description::Description(const Blade::Types blade_type, const int count) :blade_type(blade_type), count(count) {}
	PodPropeller::BladeSet::BladeSet(PodPropeller* propeller, const Types type, const double radius, const bool reversed, const double speed_ratio, const double theta_offset) :
		propeller(propeller), reversed(reversed), speed_ratio(speed_ratio), theta_offset(theta_offset) {
		const Description &description = descriptions.at(type);
		for (int i = 0; i < description.count; i++) {
			Blade* blade = new Blade(description.blade_type, radius, theta_offset + (2.0 * PI) * i / description.count, 20.0 / 180.0 * PI, reversed);
			blades.push_back(blade);
			children.push_back(blade);
		}
	}
	void PodPropeller::BladeSet::SetHeight(const double height) {
		this->height = height;
		this->SetTransform(matrix::TranslateD(glm::dvec3(0, 0, height)));
	}
	void PodPropeller::BladeSet::SetFoldState(const double fold_state) {
		for (Blade* v : blades)v->SetFoldAngle(PI / 2 * fold_state);
	}
}