#include<common/block/block.hpp>
namespace block {
	std::vector<Triangle>Block::GetTriangles(const glm::dvec3& size, const Type type)const {
		auto ans = Triangle::Cube(size / 2.0);
		for (auto& v : ans)v = v.ApplyTransform(matrix::TranslateD(size / 2.0));
		Material mtl_front,mtl_other;
		mtl_front.diffuse_texture = Material::GetTextureInfo("Picture/Block/Copper.png");
		mtl_other.diffuse_texture = Material::GetTextureInfo("Picture/Block/Soil.png");
		for (int i = 0; i < (int)ans.size(); i++) {
			ans[i].material_id = i == 2 || i == 3 ? Material::GetMaterialId("block_copper", mtl_front) : Material::GetMaterialId("block_soil", mtl_other);
		}
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
	Block::DigDirection Block::GetDigDirection()const { return dig_direction; }
	double Block::GetDigProgress()const { return dig_progress; }
	void Block::SetDigState(const DigDirection dig_direction, const double dig_progress) {
		this->dig_direction = dig_direction;
		this->dig_progress = dig_progress;
		auto it = children.begin();
		VisibleObject::Delete(*it);
		children.erase(it);
		switch (dig_direction) {
		case DigDirection::Down: {
			auto tris = GetTriangles(glm::dvec3(size.x, size.y * (1 - dig_progress), size.z), type);
			for (int i : {2, 3, 4, 5, 8, 9, 10, 11}) { // front left right back
				for (int j = 0; j < 3; j++) {
					if (tris[i].uv[j].y == 0)tris[i].uv[j].y = dig_progress;
				}
			}
			auto kid = new VisibleObject(tris);
			children.insert(kid);
			break;
		}
		case DigDirection::Left: {
			auto tris = GetTriangles(glm::dvec3(size.x * (1 - dig_progress), size.y, size.z), type);
			for (int i : {0, 1, 2, 3, 6, 7}) { // down front up
				for (int j = 0; j < 3; j++) {
					if (tris[i].uv[j].x == 1)tris[i].uv[j].x = 1 - dig_progress;
				}
			}
			for (int i : {10,11}) { // back
				for (int j = 0; j < 3; j++) {
					if (tris[i].uv[j].x == 0)tris[i].uv[j].x = dig_progress;
				}
			}
			auto kid = new VisibleObject(tris);
			children.insert(kid);
			break;
		}
		case DigDirection::Right: {
			auto tris = GetTriangles(glm::dvec3(size.x * (1 - dig_progress), size.y, size.z), type);
			for (int i : {0, 1, 2, 3, 6, 7}) { // down front up
				for (int j = 0; j < 3; j++) {
					if (tris[i].uv[j].x == 0)tris[i].uv[j].x = dig_progress;
				}
			}
			for (int i : {10, 11}) { // back
				for (int j = 0; j < 3; j++) {
					if (tris[i].uv[j].x == 1)tris[i].uv[j].x = 1 - dig_progress;
				}
			}auto kid = new VisibleObject(tris);
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