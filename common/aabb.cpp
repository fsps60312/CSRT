#include<common/aabb.hpp>
AABB::AABB() : mn(glm::vec3(FLT_MAX)), mx(glm::vec3(-FLT_MAX)) {}
void AABB::AddPoint(const glm::vec3& p) {
	mn = glm::min(mn, p);
	mx = glm::max(mx, p);
}
void AABB::AddTriangle(const std::vector<glm::vec3>& vertices, const glm::ivec3& t) {
	AddPoint(vertices[t.x]);
	AddPoint(vertices[t.y]);
	AddPoint(vertices[t.z]);
}
glm::vec3 AABB::GetMn()const {
	return mn;
}
glm::vec3 AABB::GetMx()const {
	return mx;
}
int AABB::LongestAxis()const {
	const glm::vec3 d = mx - mn;
	if (d.x >= d.y && d.x >= d.z)return 0;
	return d.y >= d.z ? 1 : 2;
}