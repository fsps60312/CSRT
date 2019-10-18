#include<common/pod/pod_drill.hpp>
namespace pod {
	std::vector<Triangle>PodDrill::Blade::GetBladeTriangles()const {
		const double r = radius / std::cos(cone_angle);
		glm::dvec3 vs[] = {
			glm::dvec3(0,0,0),
			glm::dvec3(r,0,0),
			glm::dvec3(r,r / 7,0),
			glm::dvec3(0,r / 30,0)
		};
		Material mtl;
		mtl.diffuse = glm::vec3(0.3f);
		mtl.ambient = mtl.diffuse * 0.5f;
		mtl.alpha = 1;
		const int mtl_id = Material::GetMaterialId("pod_drill_blade_basic", mtl);
		return {
			Triangle(glm::dmat3(vs[0],vs[1],vs[2]),mtl_id),
			Triangle(glm::dmat3(vs[0],vs[2],vs[3]),mtl_id),
			Triangle(glm::dmat3(vs[2],vs[1],vs[0]),mtl_id),
			Triangle(glm::dmat3(vs[3],vs[2],vs[0]),mtl_id),
		};
	}
	std::vector<Triangle>PodDrill::Blade::GetSupportTriangles()const {
		const double r = radius / std::cos(cone_angle);
		const double support_thickness = r / 100;
		Material mtl;
		mtl.diffuse = glm::vec3(0.5f);
		mtl.ambient = mtl.diffuse * 0.5f;
		mtl.alpha = 1;
		return Triangle::Cube(glm::dvec3(r / 4, support_thickness, support_thickness), Material::GetMaterialId("pod_drill_blade_support", mtl));
	}
	PodDrill::Blade::Blade(const double radius, const double rotate_angle, const double cone_angle, const double attack_angle) :
		radius(radius), rotate_angle(rotate_angle), cone_angle(cone_angle), attack_angle(attack_angle) {
		blade = new VisibleObject(GetBladeTriangles());
		support = new VisibleObject(GetSupportTriangles());
		children.insert(blade);
		children.insert(support);
		SetFoldState(0);
		SetTransform(matrix::RotateD(glm::dvec3(0, 0, 1), rotate_angle));
	}
	void PodDrill::Blade::SetFoldState(const double fold_state) {
		const double r = radius / std::cos(cone_angle);
		const double support_thickness = r / 100;
		const double angle = cone_angle + fold_state * (PI / 2 - cone_angle);
		blade->SetTransform(
			matrix::TranslateD(glm::dvec3(0.5 * r * std::cos(angle), 0, 0.5 * r * std::sin(angle))) *
			matrix::RotateD(glm::dvec3(0, 1, 0), angle) *
			matrix::TranslateD(glm::dvec3(-r / 2, 0, 0)) *
			matrix::RotateD(glm::dvec3(1, 0, 0), attack_angle)
		);
		support->SetTransform(
			matrix::RotateD(glm::dvec3(0, 1, 0), -angle) *
			matrix::TranslateD(glm::dvec3(r / 4, 0, 0))
		);
	}
}