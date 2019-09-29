#pragma once
#include<glm/glm.hpp>
#include<common/bvh_node.hpp>

#include<vector>
class VisibleObject {
private:
	const bool is_leaf;
	std::vector<glm::mat3>triangles;
	static glm::mat4 TranslateMatrix(const glm::vec3& offset);
	static glm::mat4 RotateMatrix(const glm::vec3& axis, const float theta);
	static glm::mat4 IdentityMatrix();
public:
	glm::mat4 transform = IdentityMatrix();
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