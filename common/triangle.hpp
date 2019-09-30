#pragma once
#include<glm/glm.hpp>
#include<vector>
class Triangle {
private:
	glm::mat3 vertices;
public:
	int material = 1;
	Triangle(const glm::mat3 vertices);
	glm::mat3 GetVertices()const;
	Triangle ApplyTransform(const glm::mat4& transform)const;
	static std::vector<Triangle>Cube(const glm::vec3 &xyz);
};