#pragma once
#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<vector>
namespace block {
	class Block :public  VisibleObject {
	public:
		enum Type { General };
	private:
		const glm::dvec3 position;
		Type type;
		std::vector<Triangle>GetTriangles(const glm::dvec3& size, const Type type)const;
	public:
		Block(const glm::dvec3& position, const glm::dvec3& size, const Type type);
	};
}