#include<common/material.hpp>
std::map<std::string, int>Material::material_id;
std::vector<Material>Material::glob_materials(1);
std::vector<int>Material::recycle_bin(0);
void Material::DeleteMaterialId(const std::string& name) {
	const int id = material_id.at(name);
	recycle_bin.push_back(id);
	material_id.erase(name);
}
int Material::GetMaterialId(const std::string& name) {
	return material_id.at(name);
}
int Material::GetMaterialId(const std::string& name, const Material& mtl) {
	const auto& it = material_id.find(name);
	if (it != material_id.end())return it->second;
	if (recycle_bin.empty()) {
		glob_materials.push_back(mtl);
		const int id = (int)glob_materials.size() - 1;
		material_id[name] = id;
		return id;
	} else {
		const int id = recycle_bin.back(); recycle_bin.pop_back();
		glob_materials[id] = mtl;
		material_id[name] = id;
		return id;
	}
}
Material::Material() :
	ambient(glm::vec3(0.125, 0.435, 0.25)),
	diffuse(glm::vec3(0.25, 0.87, 0.5)),
	specular(glm::vec3(0.3)),
	specular_exp(20),
	alpha(0.5) {
}