#pragma once
#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<common/control.hpp>
#include<vector>
namespace block {
	class Block :public  VisibleObject {
	public:
		enum class Type { General };
		enum class DigDirection { Down, Left, Right };
	private:
		const glm::dvec3 position;
		const glm::dvec3 size;
		Type type;
		DigDirection dig_direction = DigDirection::Down;
		double dig_progress = 0;
		std::vector<Triangle>GetTriangles(const glm::dvec3& size, const Type type)const;
	public:
		bool IsCollidable(const double dx, const double dy)const;
		void SetDigState(const DigDirection dig_direction, const double dig_progress);
		DigDirection GetDigDirection()const;
		double GetDigProgress()const;
		Block(const glm::dvec3& position, const glm::dvec3& size, const Type type);
	};
}