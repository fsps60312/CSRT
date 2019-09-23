#pragma once
#include<glm/glm.hpp>
#include<common/aabb.hpp>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
class BVHNode {
public:
#pragma region buffer
	static std::vector<glm::ivec3>glob_bvh_nodes;
	static std::vector<AABB>glob_bvh_aabbs;
	static std::vector<glm::ivec2>glob_tri_ranges;
	static std::vector<glm::mat3>glob_triangles;
	static std::vector<glm::mat4>glob_transforms;
#pragma endregion
private:
	const int id;
	const bool is_leaf;
	BVHNode* l = NULL, * r = NULL, * p = NULL;
	glm::mat4 transform = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	std::vector<glm::mat3>triangles;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
	static int GetId(BVHNode* o) { return o ? o->id : -1; }
	void NewNode();
	void Build(const int l, const int r);
	int CalMid(const int id);
	void UpdateTransform(const glm::mat4 transform);
	void UpdateAABB();
	glm::mat3 TransformedTriangle(const glm::mat3& triangle);
public:
	BVHNode(BVHNode* parent);
	BVHNode(BVHNode* parent, const std::vector<glm::mat3>& triangles);
	void SetL(BVHNode* l) { this->l = l; glob_bvh_nodes[id].x = GetId(l); }
	void SetR(BVHNode* r) { this->r = r; glob_bvh_nodes[id].y = GetId(r); }
	void SetRangeL() { glob_tri_ranges[id].x = (int)glob_triangles.size(); }
	void SetRangeR() { glob_tri_ranges[id].y = (int)glob_triangles.size() - 1; }
	void UpdateTransform();
	void Build();
	int Verify();
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};