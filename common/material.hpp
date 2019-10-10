#pragma once
#include<glm/glm.hpp>
#include<vector>
#include<map>
#include<string>
class Material {
private:
	static std::map<std::string, int>material_id;
	static std::vector<int>recycle_bin;
public:
	static std::vector<Material>glob_materials;
	static int GetMaterialId(const std::string& name);
	static void DeleteMaterialId(const std::string& name);
	static int GetMaterialId(const std::string& name, const Material& mtl);
public:
	glm::vec3 ambient, diffuse, specular;
	float specular_exp, alpha;
public:
	Material();
};