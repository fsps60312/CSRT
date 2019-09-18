#pragma once
#include<glm/glm.hpp>

#include<vector>
class VisibleObject {
private:
	std::vector<VisibleObject>children;
	std::vector<glm::ivec3>triangles;
	glm::mat4 transform;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
public:
	VisibleObject();
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};