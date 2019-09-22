#pragma once
#include<glm/glm.hpp>
#include<common/aabb.hpp>
#include<vector>
class BVHNode {
#pragma region buffer
	static std::vector<glm::ivec3>glob_bvh_nodes;
	static std::vector<AABB>glob_bvh_aabbs;
	static std::vector<glm::ivec2>glob_tri_ranges;
	static std::vector<glm::mat3>glob_triangles;
	static std::vector<glm::mat4>glob_transforms;
#pragma endregion
	int id;
	const bool is_leaf;
	BVHNode* l, * r, * p;
	std::vector<glm::mat3>triangles;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
	static int GetId(BVHNode* o) { return o ? o->id : -1; }
	void NewNode();
public:
	BVHNode(BVHNode* parent);
	BVHNode(BVHNode* parent, const std::vector<glm::mat3>& triangles);
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};