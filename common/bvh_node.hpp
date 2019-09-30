#pragma once
#include<glm/glm.hpp>
#include<common/aabb.hpp>
#include<common/triangle.hpp>
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
	static std::vector<Triangle>glob_triangles;
#pragma endregion
private:
	const int id;
	BVHNode* l = NULL, * r = NULL, * p = NULL;
	static int GetId(BVHNode* o);
	void NewNode();
	int CalMid(const int id);
	void Build(const int l, const int r);
public:
	BVHNode(BVHNode* parent);
	static void ClearVectors();
	static void DeleteTree(BVHNode* root);
	static BVHNode* Build();
	void SetL(BVHNode* l);
	void SetR(BVHNode* r);
	void SetRangeL();
	void SetRangeR();
	int Verify();
};