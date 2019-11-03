#include<common/effects/fume.hpp>
namespace effects {
	Fume::FumesParent* Fume::fumes_parent = new Fume::FumesParent();
	void Fume::FumesParent::Update(const double secs) {
		for (VisibleObject* ch : children)ch->Update(secs);
		for (VisibleObject* ch : to_erase)VisibleObject::Delete(ch), children.erase(ch);
		to_erase.clear();
	}
	void Fume::AddFume(const glm::dvec3& initial_position, const glm::dvec3& speed, const double initial_radius, const double final_radius, const double initial_alpha, const double period) {
		fumes_parent->children.insert(new Fume(initial_position, speed, initial_radius, final_radius, initial_alpha, period));
	}
	void Fume::Update(const double secs) {
		time_passed += secs;
		if (time_passed >= total_period) {
			fumes_parent->to_erase.push_back(this);
			if (material_counter != -1)Material::DeleteMaterialId("fume_" + std::to_string(material_counter));
			return;
		}
		theta = std::fmod(theta + omega * secs, 2.0 * PI);
		VisibleObject::Delete(*children.begin());
		children.erase(children.begin());
		children.insert(new VisibleObject(GetTriangles()));
		SetTransform(GetTransform());
	}
	std::vector<Triangle>Fume::GetTriangles() {
		return { Triangle(glm::dmat3(glm::dvec3(0,1,0),glm::dvec3(-0.5 * std::sqrt(3),-0.5,0),glm::dvec3(0.5 * std::sqrt(3),-0.5,0)),GetMaterial()) };
	}
	int Fume::GetMaterial() {
		const double alpha = initial_alpha * (1 - time_passed / total_period);
		Material mtl;
		mtl.alpha = alpha;
		mtl.ambient = glm::vec3(0.5);
		mtl.diffuse = glm::vec3(1);
		mtl.specular = glm::vec3(0);
		if (material_counter != -1)Material::DeleteMaterialId("fume_" + std::to_string(material_counter));
		static int counter = 0;
		const int id = Material::GetMaterialId("fume_" + std::to_string(material_counter = ++counter), mtl);
		//std::clog << "id = " << id << std::endl;
		return id;
	}
	glm::dmat4 Fume::GetTransform()const {
		const double r = time_passed / total_period;
		return matrix::TranslateD(position + speed * time_passed) * matrix::RotateD(glm::dvec3(0, 0, 1), theta) * matrix::ScaleD(glm::dvec3(initial_radius * (1 - r) + final_radius * r));
	}
	Fume::Fume(const glm::dvec3& initial_position, const glm::dvec3& speed, const double initial_radius, const double final_radius, const double initial_alpha, const double period) :
		position(initial_position),
		speed(speed),
		initial_radius(initial_radius),
		final_radius(final_radius),
		total_period(period),
		omega((mylib::Rand::NextDouble() - 0.5) * 2.0 * PI),
		time_passed(0),
		theta(mylib::Rand::NextDouble() * 2.0 * PI),
		initial_alpha(initial_alpha),
		material_counter(-1) {
		children.insert(new VisibleObject(GetTriangles()));
		SetTransform(GetTransform());
	}
}