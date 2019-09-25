#pragma once
#include<glm/glm.hpp>
#include<common/bvh_node.hpp>

#include<vector>
class VisibleObject {
private:
	const bool is_leaf;
	std::vector<glm::mat3>triangles;
	BVHNode* bvh_node;
	glm::mat4 transform = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	void Build(BVHNode* o, const int l, const int r);
	int CalMid(const int l, const int r);
	glm::mat4 TranslateMatrix(const glm::vec3& offset);
	glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
public:
	std::vector<VisibleObject*>children;
	VisibleObject();
	VisibleObject(const std::vector<glm::mat3>& triangles);
	BVHNode* Build(BVHNode* parent);
	void Update();
	void Translate(const glm::vec3& offset);
	void TranslatePrepend(const glm::vec3& offset);
	void Rotate(const glm::vec3& axis, const float theta);
	void RotatePrepend(const glm::vec3& axis, const float theta);
};