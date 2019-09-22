#pragma once
#include<glm/glm.hpp>
#include<common/bvh_node.hpp>

#include<vector>
class VisibleObject {
private:
	const bool is_leaf;
	std::vector<VisibleObject*>children;
	std::vector<glm::mat3>triangles;
	glm::mat4 transform;
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
	void Build(BVHNode* o, const int l, const int r);
	int CalMid(const int l, const int r);
public:
	VisibleObject();
	VisibleObject(const std::vector<glm::mat3>& triangles);
	BVHNode* Build(BVHNode* parent);
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};