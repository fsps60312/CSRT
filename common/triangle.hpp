#pragma once
#include<glm/glm.hpp>
#include<common/material.hpp>
#include<vector>
class Triangle {
public:
	static std::vector<Material>glob_materials;
private:
	glm::dmat3 vertices;
public:
	int material;
	Triangle(const glm::dmat3 vertices);
	glm::dmat3 GetVertices()const;
	Triangle ApplyTransform(const glm::dmat4& transform)const;
	static std::vector<Triangle>Cube(const glm::dvec3 &xyz);
};