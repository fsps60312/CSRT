#include<common/bvh_node.hpp>
#include<cmath>
std::vector<glm::ivec3>BVHNode::glob_bvh_nodes;
std::vector<AABB>BVHNode::glob_bvh_aabbs;
std::vector<glm::ivec2>BVHNode::glob_tri_ranges;
std::vector<glm::mat3>BVHNode::glob_triangles;
std::vector<glm::mat4>BVHNode::glob_transforms;
void BVHNode::NewNode() {
	id = (int)glob_bvh_nodes.size();
	glob_bvh_nodes.push_back(glm::ivec3(GetId(l), GetId(r), GetId(p)));
	glob_bvh_aabbs.push_back(AABB());
	glob_tri_ranges.push_back(glm::ivec2(-1));
	glob_transforms.push_back(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
}
BVHNode::BVHNode(BVHNode *parent) :is_leaf(false), l(NULL), r(NULL), p(parent) {
	NewNode();
}
BVHNode::BVHNode(BVHNode* parent, const std::vector<glm::mat3>& triangles) : is_leaf(true), l(NULL), r(NULL), p(parent), triangles(triangles) {
	NewNode();
}

void BVHNode::Translate(const glm::vec3& offset) {
	glob_transforms[id] = glob_transforms[id] * TranslateMatrix(offset);
}
void BVHNode::TranslatePrepend(const glm::vec3& offset) {
	glob_transforms[id] = TranslateMatrix(offset) * glob_transforms[id];
}

void BVHNode::Rotate(const glm::vec3& axis, const float theta) {
	glob_transforms[id] = glob_transforms[id] * RotateMatrix(axis, theta);
}
void BVHNode::RotatePrepend(const glm::vec3& axis, const float theta) {
	glob_transforms[id] = RotateMatrix(axis, theta) * glob_transforms[id];
}
glm::mat4 BVHNode::TranslateMatrix(const glm::vec3& offset) {
	return glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		offset.x, offset.y, offset.z, 1
	);
}
glm::mat4 BVHNode::RotateMatrix(const glm::vec3& axis, const float theta) {
	//https://zh.wikipedia.org/zh-tw/旋转矩阵
	return glm::mat4(
		std::cos(theta) + (1 - std::cos(theta)) * axis.x * axis.x,
		(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
		(1 - std::cos(theta)) * axis.x * axis.z - std::sin(theta) * axis.y,
		0,
		(1 - std::cos(theta)) * axis.y * axis.x - std::sin(theta) * axis.z,
		std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
		(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
		0,
		(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
		(1 - std::cos(theta)) * axis.z * axis.y - std::sin(theta) * axis.x,
		std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
		0,
		0, 0, 0, 1
	);
}