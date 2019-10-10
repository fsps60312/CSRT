#pragma once
#include<glm/glm.hpp>
class Material {
public:
	glm::vec3 ambient, diffuse, specular;
	float specular_exp, alpha;
public:
	Material();
};