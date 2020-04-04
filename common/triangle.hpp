#pragma once
#include<glm/glm.hpp>
#include<vector>
class Triangle {
private:
	glm::dmat3 vertices;
public:
	glm::dmat3x2 uv;
	int material_id;
	Triangle(const glm::dmat3& vertices, const int mtl_id = 0, const glm::dmat3x2& uv = glm::dmat3x2(0, 0, 0, 1, 1, 0));
	inline const glm::dmat3& GetVertices()const { return vertices; }
	Triangle ApplyTransform(const glm::dmat4& transform)const;
	static std::vector<Triangle>Cube(const glm::dvec3& xyz, const int mtl_id = 0);
};