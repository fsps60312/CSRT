#include<common/material.hpp>
std::map<std::string, int>Material::material_id;
std::vector<Material>Material::glob_materials(1);
std::vector<int>Material::recycle_bin(0);
std::vector<glm::vec4>Material::glob_textures;
std::map<std::string, glm::ivec3>Material::texture_infos;
glm::ivec3 Material::GetTextureInfo(const std::string& name) {
	const auto it = texture_infos.find(name);
	if (it != texture_infos.end())return it->second;
	int width, height, channels;
	std::clog << "loading... " << name << std::endl;
	uint8_t* raw_data = SOIL_load_image(name.c_str(), &width, &height, &channels, 4); // RGBA
	if (raw_data == NULL) {
		std::clog << "failed to load: " << name << std::endl;
		throw;
	}
	const int loc = (int)glob_textures.size();
	for (int i = 0; i < width * height; i++) {
		glm::vec4 v = glm::vec4(raw_data[i * 4], raw_data[i * 4 + 1], raw_data[i * 4 + 2], raw_data[i * 4 + 3]);
		glob_textures.push_back(v / 255.0f);
	}
	SOIL_free_image_data(raw_data);
	return texture_infos[name] = glm::ivec3(width, height, loc);
}
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
	diffuse_texture(glm::ivec3(-1)),
	alpha(0.5) {
}