#include<common/effects/gravel.hpp>
namespace effects {
	VisibleObject* Gravel::gravels_parent = new VisibleObject();
	void Gravel::AddGravel(const glm::dvec3& initial_position, const glm::dvec3& initial_speed) {
		gravels_parent->children.insert(new Gravel(initial_position, initial_speed));
	}
	std::vector<Triangle>Gravel::GetTriangles()const {
		Material mtl;
		mtl.diffuse = glm::vec3(0.6, 0.6, 0.4);
		mtl.ambient = mtl.diffuse * 0.5f;
		mtl.specular = glm::vec3(0);
		mtl.alpha = 1;
		return Triangle::Cube(glm::dvec3(1, 0.5, 0.7) *(0.1+0.2*mylib::Rand::NextDouble()), Material::GetMaterialId("gravel", mtl));
	}
	glm::dmat4 Gravel::GetTransform()const {
		return
			matrix::TranslateD(rb.position) *
			matrix::RotateD(glm::dvec3(1, 0, 0), theta_x) *
			matrix::RotateD(glm::dvec3(0, 1, 0), theta_y) *
			matrix::RotateD(glm::dvec3(0, 0, 1), theta_z);
	}
	void Gravel::Advance(const double secs) {
		rb.force = glm::dvec3(0, -constants::gravity, 0);
		rb.Advance(secs);
		double x_min, x_max, y_min, y_max;
		block::Blocks::RegionOnXYPlane(x_min, x_max, y_min, y_max);
		if (rb.position.x < x_min || x_max < rb.position.x || rb.position.y < y_min || y_max < rb.position.y) {
			gravels_parent->children.erase(this);
			VisibleObject::Delete(this);
			return;
		}
		theta_x = std::fmod(theta_x + omega_x * secs, 2.0 * PI);
		theta_y = std::fmod(theta_y + omega_y * secs, 2.0 * PI);
		theta_z = std::fmod(theta_z + omega_z * secs, 2.0 * PI);
		SetTransform(GetTransform());
	}
	Gravel::Gravel(const glm::dvec3& initial_position, const glm::dvec3& initial_speed) :
		VisibleObject(GetTriangles()),
		theta_x(mylib::Rand::NextDouble() * 2.0 * PI),
		theta_y(mylib::Rand::NextDouble() * 2.0 * PI),
		theta_z(mylib::Rand::NextDouble() * 2.0 * PI),
		omega_x((mylib::Rand::NextDouble() * 2 - 1) * 2.0 * PI),
		omega_y((mylib::Rand::NextDouble() * 2 - 1) * 2.0 * PI),
		omega_z((mylib::Rand::NextDouble() * 2 - 1) * 2.0 * PI) {
		rb.position = initial_position;
		rb.velocity = initial_speed;
		SetTransform(GetTransform());
	}
}