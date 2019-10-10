#pragma once
#include<glm/glm.hpp>
#include<vector>
class Material {
public:
	static std::vector<Material>glob_materials;
public:
	glm::vec3 ambient, diffuse, specular;
	float specular_exp, alpha;
public:
	Material();
};