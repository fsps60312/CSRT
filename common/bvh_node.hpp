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
	std::vector<glm::mat3>triangles;
	static int GetId(BVHNode* o);
	void NewNode();
	void Build(const int l, const int r);
	int CalMid(const int id);
	void UpdateTransform(const glm::mat4 transform);
	void UpdateAABB();
	glm::mat3 TransformedTriangle(const int triangle_id);
public:
	glm::mat4 transform = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	BVHNode(BVHNode* parent);
	BVHNode(BVHNode* parent, const std::vector<glm::mat3>& triangles);
	static void ClearVectors();
	static void DeleteTree(BVHNode* root);
	void SetL(BVHNode* l);
	void SetR(BVHNode* r);
	void SetRangeL();
	void SetRangeR();
	void UpdateTransform();
	void Build();
	int Verify();
};