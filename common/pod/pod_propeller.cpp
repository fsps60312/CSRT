#include<common/pod/pod_propeller.hpp>
namespace pod {
	const std::map<PodPropeller::Types,PodPropeller::Description>PodPropeller::descriptions = { {
			Types::Basic,Description({
					BladeSetDescription(BladeSet::Types::Basic,2.0,false,1),
					BladeSetDescription(BladeSet::Types::Basic,2.0,true,0.5)
				}
			)
		}
	};
	void PodPropeller::SetFoldState(const double fold_state) {
		if (fold_state < 0.7)
		{
			const double f = fold_state / 0.7;
			for (auto v : bladesets) v->SetFoldState(f);
			SetTransform(basic_transform*matrix::TranslateD(glm::dvec3(0, 0, std::sin(f * PI / 2) * (max_height - height))));
		}
		else
		{
			const double h = (1 - fold_state) / 0.3;
			for (auto v : bladesets) v->SetFoldState(1);
			SetTransform(basic_transform * matrix::TranslateD(glm::dvec3(0, 0, -height + h * max_height)));
		}
	}
	double PodPropeller::GetLiftForce() {
		return (omega - GetDownwardSpeed() * 0.05) * (1.0 - fold_state) * 5;
	}
	double PodPropeller::GetDownwardSpeed() {
		const RigidBody* rb = pod->GetRigidBody();
		return glm::dot(rb->velocity, glm::dvec3(-std::sin(rb->theta), std::cos(rb->theta), 0.0));
	}
	double PodPropeller::GetAirFriction(const double air_speed){
		return 0.5 * std::pow(air_speed, 2) + 0.01 * std::abs(air_speed);
	}
	PodPropeller::PodPropeller(PodInterface* pod, const Types propeller_type, const glm::dmat4& basic_transform) :pod(pod), basic_transform(basic_transform) {
		for (const auto& bladeset_description : descriptions.at(propeller_type).bladesets) {
			BladeSet* bladeset = new BladeSet(
				this,
				bladeset_description.type,
				bladeset_description.radius,
				bladeset_description.reversed,
				bladeset_description.speed_ratio,
				bladeset_description.theta_offset
			);
			bladeset->SetHeight(bladeset_description.height);
			bladesets.push_back(bladeset);
			children.push_back(bladeset);
		}
		height = max_height = 0;
		for (const auto& v : descriptions.at(propeller_type).bladesets) height = std::max(height, v.height);
		for (const auto& v : descriptions.at(propeller_type).bladesets) max_height = std::max(max_height, std::max(0.0, v.radius - v.height) + height);
		const double pole_radius = 0.05;
		const double z_min = height - max_height, z_max = height;
		VisibleObject* pole = new VisibleObject(Triangle::Cube(glm::dvec3(pole_radius, pole_radius, 0.5 * (z_max - z_min))));
		pole->Translate(glm::dvec3(0, 0, 0.5 * (z_max - z_min) + z_min));
		children.push_back(pole);
		SetFoldState(0);
	}
	PodPropeller::BladeSetDescription::BladeSetDescription(const BladeSet::Types type, const double radius, const bool reversed, const double height, const double speed_ratio, const double theta_offset) :
		type(type), radius(radius), reversed(reversed), height(height), speed_ratio(speed_ratio), theta_offset(theta_offset) {}
	PodPropeller::Description::Description(const std::vector<BladeSetDescription>& bladesets) : bladesets(bladesets) {}
}