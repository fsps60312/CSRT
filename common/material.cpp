#include<common/material.hpp>
Material::Material() :
	ambient(glm::vec3(0.125, 0.435, 0.25)),
	diffuse(glm::vec3(0.25, 0.87, 0.5)),
	specular(glm::vec3(0.3)),
	specular_exp(20),
	alpha(1) {
}