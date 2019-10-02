#include<common/pod/pod_propeller.hpp>
namespace pod {
	void PodPropeller::BladeSet::Blade::SetFoldAngle(const double theta) {
		fold_angle = theta;
	}
	std::vector<Triangle>PodPropeller::BladeSet::Blade::GetTriangles(const Types type, const double radius, double theta, const double reversed) {
		std::vector<glm::dvec3>vs = descriptions.at(type).vertices;
		if (reversed) {
			for (auto& v : vs)v *= radius;
			theta = -theta;
		}
		std::vector<Triangle>ret;
		ret.push_back(Triangle(glm::mat3(vs[3], vs[2], vs[0])));
		ret.push_back(Triangle(glm::mat3(vs[2], vs[1], vs[0])));
		ret.push_back(Triangle(glm::mat3(vs[0], vs[1], vs[2])));
		ret.push_back(Triangle(glm::mat3(vs[0], vs[2], vs[3])));
		return ret;
	}
	PodPropeller::BladeSet::Blade::Blade(const Types type, const double radius, const double theta, const double reversed) :VisibleObject(GetTriangles(type, radius, theta, reversed)) {}
	PodPropeller::BladeSet::Blade::Description::Description(const std::vector<glm::dvec3>& vertices) : vertices(vertices) {}
}