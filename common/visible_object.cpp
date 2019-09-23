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

void VisibleObject::Translate(const glm::vec3& offset) {
	bvh_node->Translate(offset);
}
void VisibleObject::TranslatePrepend(const glm::vec3& offset) {
	bvh_node->TranslatePrepend(offset);
}

void VisibleObject::Rotate(const glm::vec3& axis, const float theta) {
	bvh_node->Rotate(axis, theta);
}
void VisibleObject::RotatePrepend(const glm::vec3& axis, const float theta) {
	bvh_node->RotatePrepend(axis, theta);
}
void VisibleObject::Update() {
	bvh_node->UpdateTransform();
}