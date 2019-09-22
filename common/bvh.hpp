#pragma once
#include<glm/glm.hpp>
#include<common/aabb.hpp>
#include<common/visible_object.hpp>

#include<vector>
#include<algorithm>
#include<iostream>

class BVH {
private:
	std::vector<glm::ivec3>nodes;
	std::vector<glm::mat3>triangles;
	std::vector<AABB>aabbs;
	std::vector<glm::ivec2>ranges;
	void Build(const int id, const int l, const int r);
	int CalMid(std::vector<glm::mat3>& triangles, const int id)const;
	void TryTraverse(const int id, int& counter);
public:
	BVH(){}
	BVH(const std::vector<glm::mat3>& triangles);
	void Build();
	std::vector<glm::mat3>GetTriangles() { return triangles; }
	std::vector<glm::ivec3>GetNodes() { return nodes; }
	std::vector<AABB>GetAabbs() { return aabbs; }
	std::vector<glm::ivec2>GetRanges() { return ranges; }
};