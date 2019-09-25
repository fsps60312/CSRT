#include<common/visible_object.hpp>
#include<cmath>
VisibleObject::VisibleObject() :
	is_leaf(false),
	bvh_node(NULL) {
}
VisibleObject::VisibleObject(const std::vector<glm::mat3>& triangles) :
	is_leaf(true),
	triangles(triangles),
	bvh_node(NULL) {
}
BVHNode* VisibleObject::Build(BVHNode* parent) {
	if (is_leaf) {
		bvh_node = new BVHNode(parent, triangles);
	} else {
		if (children.empty())return NULL;
		bvh_node = new BVHNode(parent);
		Build(bvh_node, 0, (int)children.size() - 1);
	}
	return bvh_node;
}
void VisibleObject::Build(BVHNode* o, const int l, const int r) {
	assert(o != NULL);
	o->SetRangeL();
	if (l == r) {
		o->SetL(children[l]->Build(o));
	} else if (l + 1 == r) {
		o->SetL(children[l]->Build(o));
		o->SetR(children[r]->Build(o));
	} else {
		BVHNode* lch = new BVHNode(o), * rch = new BVHNode(o);
		const int mid = CalMid(l, r);
		Build(lch, l, mid);
		Build(rch, mid + 1, r);
		o->SetL(lch);
		o->SetR(rch);
	}
	o->SetRangeR();
}
int VisibleObject::CalMid(const int l, const int r) {
	return (l + r) / 2;
}

void VisibleObject::Update() {
	bvh_node->transform = this->transform;
	for (auto child : children)child->Update();
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
		(1 - std::cos(theta)) * axis.y * axis.x + std::sin(theta) * axis.z,
		(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
		0,
		(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
		std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
		(1 - std::cos(theta)) * axis.z * axis.y + std::sin(theta) * axis.x,
		0,
		(1 - std::cos(theta)) * axis.x * axis.z + std::sin(theta) * axis.y,
		(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
		std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
		0,
		0, 0, 0, 1
	);
	return glm::mat4(
		std::cos(theta) + (1 - std::cos(theta)) * axis.x * axis.x,
		(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
		(1 - std::cos(theta)) * axis.x * axis.z + std::sin(theta) * axis.y,
		0,
		(1 - std::cos(theta)) * axis.y * axis.x + std::sin(theta) * axis.z,
		std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
		(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
		0,
		(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
		(1 - std::cos(theta)) * axis.z * axis.y + std::sin(theta) * axis.x,
		std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
		0,
		0, 0, 0, 1
	);
}