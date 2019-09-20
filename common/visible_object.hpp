#pragma once
#include<glm/glm.hpp>

#include<vector>
class VisibleObject {
private:
	std::vector<VisibleObject*>children;
	int L, R;
	glm::mat4 transform;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
public:
	VisibleObject(const int l, const int r);
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
	int GetL()const { return L; }
	int GetR()const { return R; }
	std::vector<VisibleObject*>& GetChildren() { return children; }
};