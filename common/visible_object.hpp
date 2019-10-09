#pragma once
#include<glm/glm.hpp>
#include<common/bvh_node.hpp>
#include<common/triangle.hpp>
#include<common/matrix_d.hpp>

#include<vector>
#include<set>
class VisibleObject {
private:
	const bool is_leaf;
	std::vector<Triangle>triangles;
	glm::dmat4 transform = matrix::IdentityD();
protected:
public:
	void SetTransform(const glm::dmat4& t);
	std::set<VisibleObject*>children;
	VisibleObject();
	VisibleObject(const std::vector<Triangle>& triangles);
	virtual void PrepareForRound();
	virtual void Update(const double secs);
	virtual void Advance(const double secs);
	static void Delete(const VisibleObject* o);
	void Build(const glm::dmat4& transform)const;
	void Translate(const glm::dvec3& offset);
	void TranslatePrepend(const glm::dvec3& offset);
	void Rotate(const glm::dvec3& axis, const float theta);
	void RotatePrepend(const glm::dvec3& axis, const float theta);
};