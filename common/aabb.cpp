#include<common/aabb.hpp>
AABB::AABB() : mn(glm::vec3(FLT_MAX)), mx(glm::vec3(-FLT_MAX)) {}
void AABB::AddPoint(const glm::vec3& p) {
	assert(!glm::any(glm::isnan(p)));
	mn = glm::min(mn, p);
	mx = glm::max(mx, p);
}
void AABB::AddTriangle(const Triangle& tri) {
	const auto& t = tri.GetVertices();
	AddPoint(t[0]);
	AddPoint(t[1]);
	AddPoint(t[2]);
}
void AABB::AddAABB(const AABB& aabb) {
	mn = glm::min(mn, aabb.mn);
	mx = glm::max(mx, aabb.mx);
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