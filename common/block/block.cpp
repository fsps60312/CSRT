#include<common/block/block.hpp>
namespace block {
	std::vector<Triangle>Block::GetTriangles(const glm::dvec3& size, const Type type)const {
		auto ans = Triangle::Cube(size / 2.0);
		for (auto& v : ans)v = v.ApplyTransform(matrix::TranslateD(size / 2.0));
		return ans;
	}
	Block::Block(const glm::dvec3& position, const glm::dvec3& size, const Type type) :
		VisibleObject(GetTriangles(size, type)),
		position(position), type(type) {
		SetTransform(matrix::TranslateD(position));
	}
}