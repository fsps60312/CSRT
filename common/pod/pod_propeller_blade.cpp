#include<common/pod/pod_propeller.hpp>
namespace pod {
	const std::map<PodPropeller::BladeSet::Blade::Types, PodPropeller::BladeSet::Blade::Description> PodPropeller::BladeSet::Blade::descriptions = {
		{Types::BasicTriangle,Description({mylib::FromAngular(0,0),mylib::FromAngular(1,0),mylib::FromAngular(0.9,-7.0 / 180 * PI),mylib::FromAngular(0.2,-40.0 / 180 * PI)})}
	};
	void PodPropeller::BladeSet::Blade::SetFoldAngle(const double theta) {
		fold_angle = theta;
		SetTransform(GetTransform());
	}
	glm::dmat4 PodPropeller::BladeSet::Blade::GetTransform()const {
		return matrix::RotateD(glm::dvec3(0, 0, 1), cycle_angle) * matrix::RotateD(glm::dvec3(0, 1, 0), fold_angle) * matrix::RotateD(glm::dvec3(1, 0, 0), turn_down_angle);
	}
	std::vector<Triangle>PodPropeller::BladeSet::Blade::GetTriangles(const Types type, const double radius, const double reversed) {
		//std::clog << "description.size() = " << descriptions.size() << std::endl;
		const Description& description = descriptions.at(type);
		std::vector<glm::dvec3>vs = description.vertices;
		for (auto& v : vs)v *= radius;
		if (reversed) {
			for (auto& v : vs)v.y = -v.y;
			std::reverse(vs.begin(), vs.end());
		}
		std::vector<Triangle>ret;
		ret.push_back(Triangle(glm::mat3(vs[3], vs[2], vs[0])));
		ret.push_back(Triangle(glm::mat3(vs[0], vs[2], vs[3])));
		ret.push_back(Triangle(glm::mat3(vs[2], vs[1], vs[0])));
		ret.push_back(Triangle(glm::mat3(vs[0], vs[1], vs[2])));
		return ret;
	}
	PodPropeller::BladeSet::Blade::Blade(const Types type, const double radius, const double cycle_angle, const double turn_down_angle, const double reversed) :
		VisibleObject(GetTriangles(type, radius, reversed)),
		cycle_angle(reversed ? -cycle_angle : cycle_angle), turn_down_angle(reversed ? -turn_down_angle : turn_down_angle) {
		//SetTransform(GetTransform());
	}
	PodPropeller::BladeSet::Blade::Description::Description(const std::vector<glm::dvec3>& vertices) : vertices(vertices) {}
}