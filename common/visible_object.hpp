#pragma once
#include<glm/glm.hpp>
#include<common/bvh_node.hpp>

#include<vector>
class VisibleObject {
private:
	const bool is_leaf;
	std::vector<glm::mat3>triangles;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
public:
	glm::mat4 transform = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	std::vector<VisibleObject*>children;
	VisibleObject();
	VisibleObject(const std::vector<glm::mat3>& triangles);
	virtual void Advance(const double secs);
	void Build(const glm::mat4& transform)const;
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};