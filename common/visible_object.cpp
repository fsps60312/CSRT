#include<common/visible_object.hpp>
#include<cmath>
VisibleObject::VisibleObject():
	transform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)) {
}

void VisibleObject::Translate(const glm::vec3& offset) {
	transform = transform * TranslateMatrix(offset);
}
void VisibleObject::TranslatePrepend(const glm::vec3& offset) {
	transform = TranslateMatrix(offset) * transform;
}

void VisibleObject::Rotate(const glm::vec3& axis, const float theta) {
	transform = transform * RotateMatrix(axis, theta);
}
void VisibleObject::RotatePrepend(const glm::vec3& axis, const float theta) {
	transform = RotateMatrix(axis, theta) * transform;
}
glm::mat4 VisibleObject::TranslateMatrix(const glm::vec3& offset) {
	return glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		offset.x, offset.y, offset.z, 1
	);
}
glm::mat4 VisibleObject::RotateMatrix(const glm::vec3& axis, const float theta) {
	//https://zh.wikipedia.org/zh-tw/旋转矩阵
	return glm::mat4(
		std::cos(theta) + (1 - std::cos(theta)) * axis.x * axis.x,
		(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
		(1 - std::cos(theta)) * axis.x * axis.z - std::sin(theta) * axis.y,
		0,
		(1 - std::cos(theta)) * axis.y * axis.x - std::sin(theta) * axis.z,
		std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
		(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
		0,
		(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
		(1 - std::cos(theta)) * axis.z * axis.y - std::sin(theta) * axis.x,
		std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
		0,
		0, 0, 0, 1
	);
}