#include<common/triangle.hpp>
Triangle::Triangle(const glm::dmat3& vertices) :vertices(vertices), material_id(0) {}
Triangle::Triangle(const glm::dmat3& vertices, const int mtl_id) : vertices(vertices), material_id(mtl_id) {}
glm::dmat3 Triangle::GetVertices()const { return vertices; }
Triangle Triangle::ApplyTransform(const glm::dmat4& transform)const {
	glm::dmat3 ret;
	const glm::dmat3& t = GetVertices();
	for (int i = 0; i < 3; i++) {
		auto res = (transform * glm::dvec4(t[i], 1.0f));
		assert(!glm::any(glm::isnan(res)));
		ret[i].x = res.x;
		ret[i].y = res.y;
		ret[i].z = res.z;
	}
	return Triangle(ret, material_id);
}
std::vector<Triangle>Triangle::Cube(const glm::dvec3 &xyz,const int mtl_id) {
	glm::dvec3 ps[8] = {
		glm::dvec3(-xyz.x,-xyz.y, xyz.z),
		glm::dvec3( xyz.x,-xyz.y, xyz.z),
		glm::dvec3(-xyz.x,-xyz.y,-xyz.z),
		glm::dvec3( xyz.x,-xyz.y,-xyz.z),
		glm::dvec3(-xyz.x, xyz.y, xyz.z),
		glm::dvec3( xyz.x, xyz.y, xyz.z),
		glm::dvec3(-xyz.x, xyz.y,-xyz.z),
		glm::dvec3( xyz.x, xyz.y,-xyz.z),
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
		Triangle(glm::dmat3(ps[0],ps[2],ps[3]),mtl_id),
		Triangle(glm::dmat3(ps[0],ps[3],ps[1]),mtl_id),
		Triangle(glm::dmat3(ps[0],ps[1],ps[5]),mtl_id),
		Triangle(glm::dmat3(ps[0],ps[5],ps[4]),mtl_id),
		Triangle(glm::dmat3(ps[0],ps[4],ps[6]),mtl_id),
		Triangle(glm::dmat3(ps[0],ps[6],ps[2]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[6],ps[4]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[4],ps[5]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[5],ps[1]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[1],ps[3]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[3],ps[2]),mtl_id),
		Triangle(glm::dmat3(ps[7],ps[2],ps[6]),mtl_id)
	};
}