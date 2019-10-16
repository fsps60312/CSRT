#pragma once
#include<glm/glm.hpp>
#include<SOIL.h>
#include<vector>
#include<map>
#include<string>
#include<iostream>
class Material {
private:
	static std::map<std::string, int>material_id;
	static std::vector<int>recycle_bin;
	static std::map<std::string, glm::ivec3>texture_infos;
public:
	static std::vector<Material>glob_materials;
	static std::vector<glm::vec4>glob_textures;
	static int GetMaterialId(const std::string& name);
	static void DeleteMaterialId(const std::string& name);
	static int GetMaterialId(const std::string& name, const Material& mtl);
	static glm::ivec3 GetTextureInfo(const std::string& name);
public:
	glm::vec3 ambient, diffuse, specular;
	float specular_exp, alpha;
	glm::ivec3 diffuse_texture;
public:
	Material();
};