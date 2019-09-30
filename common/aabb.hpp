#pragma once
#include<glm/glm.hpp>
#include<common/triangle.hpp>

#include<limits>
#include<vector>

class AABB {
private:
	glm::vec3 mn, mx;
public:
	AABB();
	void AddPoint(const glm::vec3& p);
	void AddTriangle(const Triangle& tri);
	void AddAABB(const AABB& aabb);
	glm::vec3 GetMn()const;
	glm::vec3 GetMx()const;
	int LongestAxis()const;
};