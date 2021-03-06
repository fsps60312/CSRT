#include<common/triangle.hpp>
Triangle::Triangle(const glm::dmat3& vertices, const int mtl_id, const glm::dmat3x2& uv) : vertices(vertices), material_id(mtl_id), uv(uv) {}
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
	return Triangle(ret, material_id, uv);
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
		Triangle(glm::dmat3(ps[0],ps[2],ps[3]),mtl_id,glm::dmat3x2(0,0,0,1,1,1)),//down
		Triangle(glm::dmat3(ps[0],ps[3],ps[1]),mtl_id,glm::dmat3x2(0,0,1,1,1,0)),
		Triangle(glm::dmat3(ps[0],ps[1],ps[5]),mtl_id,glm::dmat3x2(0,1,1,1,1,0)),//front
		Triangle(glm::dmat3(ps[0],ps[5],ps[4]),mtl_id,glm::dmat3x2(0,1,1,0,0,0)),
		Triangle(glm::dmat3(ps[0],ps[4],ps[6]),mtl_id,glm::dmat3x2(1,1,1,0,0,0)),//left
		Triangle(glm::dmat3(ps[0],ps[6],ps[2]),mtl_id,glm::dmat3x2(1,1,0,0,0,1)),
		Triangle(glm::dmat3(ps[7],ps[6],ps[4]),mtl_id,glm::dmat3x2(1,0,0,0,0,1)),//up
		Triangle(glm::dmat3(ps[7],ps[4],ps[5]),mtl_id,glm::dmat3x2(1,0,0,1,1,1)),
		Triangle(glm::dmat3(ps[7],ps[5],ps[1]),mtl_id,glm::dmat3x2(1,0,0,0,0,1)),//right
		Triangle(glm::dmat3(ps[7],ps[1],ps[3]),mtl_id,glm::dmat3x2(1,0,0,1,1,1)),
		Triangle(glm::dmat3(ps[7],ps[3],ps[2]),mtl_id,glm::dmat3x2(0,0,0,1,1,1)),//back
		Triangle(glm::dmat3(ps[7],ps[2],ps[6]),mtl_id,glm::dmat3x2(0,0,1,1,1,0))
	};
}