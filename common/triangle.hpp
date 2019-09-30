#pragma once
#include<glm/glm.hpp>
#include<vector>
class Triangle {
private:
	glm::mat3 vertices;
public:
	Triangle(const glm::mat3 vertices);
	static std::vector<Triangle>Cube(const glm::vec3 &xyz);
};