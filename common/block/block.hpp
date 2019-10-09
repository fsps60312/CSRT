#pragma once
#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<vector>
namespace block {
	class Block :public  VisibleObject {
	public:
		enum Type { General };
		enum DigDirection { Down, Left, Right };
	private:
		const glm::dvec3 position;
		const glm::dvec3 size;
		Type type;
		std::vector<Triangle>GetTriangles(const glm::dvec3& size, const Type type)const;
	public:
		void SetDigState(const DigDirection dig_direction, const double remain_ratio);
		Block(const glm::dvec3& position, const glm::dvec3& size, const Type type);
	};
}