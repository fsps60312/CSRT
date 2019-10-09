#include<common/visible_object.hpp>
#include<cmath>
VisibleObject::VisibleObject() :
	is_leaf(false) {
}
VisibleObject::VisibleObject(const std::vector<Triangle>& triangles) :
	is_leaf(true),
	triangles(triangles) {
}
void VisibleObject::Delete(const VisibleObject* o) {
	if (o == NULL)return;
	for (const VisibleObject* ch : o->children)Delete(ch);
	delete o;
}
void VisibleObject::PrepareForRound() {
	for (VisibleObject* ch : children)ch->PrepareForRound();
}
void VisibleObject::Update(const double secs) {
	for (VisibleObject* ch : children)ch->Update(secs);
}
void VisibleObject::SetTransform(const glm::dmat4& t) {
	assert(!glm::any(glm::isnan(t[0])) && !glm::any(glm::isnan(t[1])) && !glm::any(glm::isnan(t[2])) && !glm::any(glm::isnan(t[3])));
	assert(!glm::any(glm::isinf(t[0])) && !glm::any(glm::isinf(t[1])) && !glm::any(glm::isinf(t[2])) && !glm::any(glm::isinf(t[3])));
	transform = t;
}
void VisibleObject::Advance(const double secs) {
	for (VisibleObject* ch : children)ch->Advance(secs);
}
void VisibleObject::Build(const glm::dmat4 &parent_transform)const {
	const glm::dmat4& transform = parent_transform * this->transform;
	if (is_leaf) {
		for (const auto& t : triangles) {
			BVHNode::glob_triangles.push_back(t.ApplyTransform(transform));
		}
	} else {
		for (const auto ch : children)ch->Build(transform);
	}
}

void VisibleObject::Translate(const glm::dvec3& offset) {
	transform = transform * matrix::TranslateD(offset);
}
void VisibleObject::TranslatePrepend(const glm::dvec3& offset) {
	transform = matrix::TranslateD(offset) * transform;
}

void VisibleObject::Rotate(const glm::dvec3& axis, const float theta) {
	transform = transform * matrix::RotateD(axis, theta);
}
void VisibleObject::RotatePrepend(const glm::dvec3& axis, const float theta) {
	transform = matrix::RotateD(axis, theta) * transform;
}