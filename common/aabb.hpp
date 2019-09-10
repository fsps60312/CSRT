#include<glm/glm.hpp>

#include<limits>
#include<vector>

class AABB {
private:
	glm::vec3 mn, mx;
public:
	AABB();
	void AddPoint(const glm::vec3& p);
	void AddTriangle(const std::vector<glm::vec3>& vertices, const glm::ivec3& t);
	glm::vec3 GetMn()const;
	glm::vec3 GetMx()const;
	int LongestAxis()const;
};