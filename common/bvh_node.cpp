#include<common/bvh_node.hpp>
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
	//glob_transforms.back() = RotateMatrix(glm::vec3(0, 0, 1), glm::acos(-1) / 6);
}

BVHNode::BVHNode(BVHNode *parent) :l(NULL), r(NULL), p(parent) {
	NewNode();
}

BVHNode::BVHNode(BVHNode* parent, const std::vector<glm::mat3>& triangles) :l(NULL), r(NULL), p(parent), triangles(triangles) {
	NewNode();
}

void BVHNode::Build(const int l, const int r) {
	// range
	glob_tri_ranges[id] = glm::ivec2(l, r);
	// cal aabb
	auto& aabb = glob_bvh_aabbs[id] = AABB();
	for (int i = l; i <= r; i++)aabb.AddTriangle(glob_triangles[i]);
	if (r - l + 1 <= 2) {
		return;
	}
	// split
	const int mid = CalMid(id);
	// dfs
	BVHNode* lch = new BVHNode(this), * rch = new BVHNode(this);
	lch->Build(l, mid);
	rch->Build(mid + 1, r);
	SetL(lch);
	SetR(rch);
}
int BVHNode::CalMid(const int id) {
	auto dot_max = [](const glm::mat3& t, const glm::vec3& c)->float {
		return std::min(std::min(glm::dot(t[0], c), glm::dot(t[1], c)), glm::dot(t[2], c));
	};
	auto surface_area = [](const AABB& aabb)->float {
		const glm::vec3 d = aabb.GetMx() - aabb.GetMn();
		return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
	};
	// sort along longest axis
	glm::vec3 c(0); c[glob_bvh_aabbs[id].LongestAxis()] = 1;
	const int l = glob_tri_ranges[id].x, r = glob_tri_ranges[id].y;
	std::sort(glob_triangles.begin() + l, glob_triangles.begin() + r + 1, [&c, &dot_max](const glm::mat3& t1, const glm::mat3& t2)->bool {
		return dot_max(t1, c) < dot_max(t2, c);
	});
	// left aabbs
	std::vector<AABB>left_aabbs(long long(r) - l);
	AABB left_aabb;
	for (long long i = l; i < r; i++) {
		left_aabb.AddTriangle(glob_triangles[i]);
		left_aabbs[i - l] = left_aabb;
	}
	int best_mid = -1;
	float best_sah_cost = FLT_MAX;
	// rigt aabbs
	AABB rigt_aabb;
	for (long long i = r; i > l; i--) {
		rigt_aabb.AddTriangle(glob_triangles[i]);
		const float sah_cost = (i - l) * surface_area(left_aabbs[i - l - 1]) + (r - i + 1) * surface_area(rigt_aabb);
		if (sah_cost < best_sah_cost) {
			best_sah_cost = sah_cost;
			best_mid = i - 1;
		}
	}
	assert(best_mid != -1);
	return best_mid;
}

void BVHNode::Build() {
	auto& aabb = glob_bvh_aabbs[id] = AABB();
	glob_tri_ranges[id].x = (int)glob_triangles.size();
	if (l == NULL && r == NULL) {
		for (const auto t : triangles) {
			glob_triangles.push_back(t);
			aabb.AddTriangle(t);
		}
		std::clog << "triangles.size=" << triangles.size() << std::endl;
		Build(glob_tri_ranges[id].x, (int)glob_triangles.size() - 1);
	} else {
		if (l != NULL) {
			l->Build();
			aabb.AddAABB(glob_bvh_aabbs[l->id]);
		}
		if (r != NULL) {
			r->Build();
			aabb.AddAABB(glob_bvh_aabbs[r->id]);
		}
	}
	glob_tri_ranges[id].y = (int)glob_triangles.size() - 1;
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
		(1 - std::cos(theta)) * axis.y * axis.x + std::sin(theta) * axis.z,
		(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
		0,
		(1 - std::cos(theta))* axis.x* axis.y - std::sin(theta) * axis.z,
		std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
		(1 - std::cos(theta)) * axis.z * axis.y + std::sin(theta) * axis.x,
		0,
		(1 - std::cos(theta))* axis.x* axis.z + std::sin(theta) * axis.y,
		(1 - std::cos(theta))* axis.y* axis.z - std::sin(theta) * axis.x,
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