#include<common/block/block.hpp>
namespace block {
	std::vector<Triangle>Block::GetTriangles(const glm::dvec3& size, const Type type)const {
		auto ans = Triangle::Cube(size / 2.0);
		for (auto& v : ans)v = v.ApplyTransform(matrix::TranslateD(size / 2.0));
		return ans;
	}
	bool Block::IsCollidable(const double dx, const double dy)const {
		switch (dig_direction) {
		case DigDirection::Down: {
			const double line = constants::block_height * (1 - dig_progress);
			return dy <= line;
		}
		case DigDirection::Left: {
			const double line = constants::block_width * (1 - dig_progress);
			return dx <= line;
		}
		case DigDirection::Right: {
			const double line = constants::block_width * dig_progress;
			return dx >= line;
		}
		default:throw;
		}
	}
	void Block::SetDigState(const DigDirection dig_direction, const double dig_progress) {
		this->dig_direction = dig_direction;
		this->dig_progress = dig_progress;
		auto it = children.begin();
		VisibleObject::Delete(*it);
		children.erase(it);
		switch (dig_direction) {
		case DigDirection::Down: {
			auto kid = new VisibleObject(GetTriangles(glm::dvec3(size.x, size.y * (1 - dig_progress), size.z), type));
			children.insert(kid);
			break;
		}
		case DigDirection::Left: {
			auto kid = new VisibleObject(GetTriangles(glm::dvec3(size.x * (1 - dig_progress), size.y, size.z), type));
			children.insert(kid);
			break;
		}
		case DigDirection::Right: {
			auto kid = new VisibleObject(GetTriangles(glm::dvec3(size.x * (1 - dig_progress), size.y, size.z), type));
			kid->Translate(glm::dvec3(size.x * dig_progress, 0, 0));
			children.insert(kid);
			break;
		}
		default:throw;
		}
	}
	Block::Block(const glm::dvec3& position, const glm::dvec3& size, const Type type) :
		position(position), size(size), type(type) {
		SetTransform(matrix::TranslateD(position));
		children.insert(new VisibleObject(GetTriangles(size, type)));
	}
}