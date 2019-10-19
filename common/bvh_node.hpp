#pragma once
#include<glm/glm.hpp>
#include<common/aabb.hpp>
#include<common/triangle.hpp>
#include<common/material.hpp>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<mutex>
#include<thread>
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
	static std::mutex new_node_lock;
	void NewNode();
	int CalMid(const int id, std::vector<int>& tri_ids);
	void Build(const int l, const int r,const int depth, std::vector<int>& tri_ids);
public:
	BVHNode(BVHNode* parent, const int id);
	static void ClearVectors();
	static void DeleteTree(BVHNode* root);
	static BVHNode* Build();
	void SetL(BVHNode* l);
	void SetR(BVHNode* r);
	void SetRangeL();
	void SetRangeR();
	int Verify(int depth, int& max_depth);
};