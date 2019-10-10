#pragma once
#include<glm/glm.hpp>
#include<vector>
class Triangle {
private:
	glm::dmat3 vertices;
public:
	int material_id;
	Triangle(const glm::dmat3 vertices);
	glm::dmat3 GetVertices()const;
	Triangle ApplyTransform(const glm::dmat4& transform)const;
	static std::vector<Triangle>Cube(const glm::dvec3 &xyz);
};