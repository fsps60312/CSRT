#pragma once
#include<glm/glm.hpp>

#include<limits>
#include<vector>

class AABB {
private:
	glm::vec3 mn, mx;
public:
	AABB();
	void AddPoint(const glm::vec3& p);
	void AddTriangle(const glm::mat3& t);
	glm::vec3 GetMn()const;
	glm::vec3 GetMx()const;
	int LongestAxis()const;
};