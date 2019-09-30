#include<common/triangle.hpp>
Triangle::Triangle(const glm::mat3 vertices) :vertices(vertices) {}
glm::mat3 Triangle::GetVertices()const { return vertices; }
Triangle Triangle::ApplyTransform(const glm::mat4& transform)const {
	glm::mat3 ret;
	const glm::mat3& t = GetVertices();
	for (int i = 0; i < 3; i++) {
		auto res = (transform * glm::vec4(t[i], 1.0f));
		ret[i].x = res.x;
		ret[i].y = res.y;
		ret[i].z = res.z;
	}
	return Triangle(ret);
}
std::vector<Triangle>Triangle::Cube(const glm::vec3 &xyz) {
	glm::vec3 ps[8] = {
		glm::vec3(-xyz.x,-xyz.y, xyz.z),
		glm::vec3( xyz.x,-xyz.y, xyz.z),
		glm::vec3(-xyz.x,-xyz.y,-xyz.z),
		glm::vec3( xyz.x,-xyz.y,-xyz.z),
		glm::vec3(-xyz.x, xyz.y, xyz.z),
		glm::vec3( xyz.x, xyz.y, xyz.z),
		glm::vec3(-xyz.x, xyz.y,-xyz.z),
		glm::vec3( xyz.x, xyz.y,-xyz.z),
	};
	/*
	  o----o
	 /    /|
	o----o |
	|    | o
	|    |/
	o----o
	*/
	return {
		glm::mat3(ps[0],ps[2],ps[3]),
		glm::mat3(ps[0],ps[3],ps[1]),
		glm::mat3(ps[0],ps[1],ps[5]),
		glm::mat3(ps[0],ps[5],ps[4]),
		glm::mat3(ps[0],ps[4],ps[6]),
		glm::mat3(ps[0],ps[6],ps[2]),
		glm::mat3(ps[7],ps[6],ps[4]),
		glm::mat3(ps[7],ps[4],ps[5]),
		glm::mat3(ps[7],ps[5],ps[1]),
		glm::mat3(ps[7],ps[1],ps[3]),
		glm::mat3(ps[7],ps[3],ps[2]),
		glm::mat3(ps[7],ps[2],ps[6])
	};
}