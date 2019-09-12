#include<common/bvh.hpp>

BVH::BVH(const std::vector<glm::ivec3>& triangles) {
	this->triangles = triangles;
}
void BVH::Build(const std::vector<glm::vec3>& vertices) {
	const size_t desired_size = vertices.size() * 2 - 1;
	nodes.resize(desired_size);
	aabbs.resize(desired_size);
	ranges.resize(desired_size);
	Build(vertices, 0, 0, (int)triangles.size() - 1);
}
void BVH::Build(const std::vector<glm::vec3>& vertices, const int id, const int l, const int r) {
	// range
	ranges[id] = glm::ivec2(l, r);
	// cal aabb
	auto& aabb = aabbs[id] = AABB();
	for (int i = l; i <= r; i++)aabb.AddTriangle(vertices, triangles[i]);
	// split
	const int mid = CalMid(vertices, triangles, id);
	// dfs
	const int chsz = mid - l + 1;
	Build(vertices, id + 1, l, mid);
	Build(vertices, id + chsz * 2, mid + 1, r);
}
int BVH::CalMid(const std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& triangles, const int id)const {
	auto dot_max = [&vertices](const glm::ivec3& t, const glm::vec3& c)->float {
		return std::min(std::min(glm::dot(vertices[t.x], c), glm::dot(vertices[t.y], c)), glm::dot(vertices[t.z], c));
	};
	auto surface_area = [](const AABB& aabb)->float {
		const glm::vec3 d = aabb.GetMx() - aabb.GetMn();
		return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
	};
	// sort along longest axis
	glm::vec3 c(0); c[aabbs[id].LongestAxis()] = 1;
	const int l = ranges[id].x, r = ranges[id].y;
	std::sort(triangles.begin() + l, triangles.begin() + r + 1, [&c, &dot_max](const glm::ivec3& t1, const glm::ivec3& t2)->bool {
		return dot_max(t1, c) < dot_max(t2, c);
	});
	// left aabbs
	std::vector<AABB>left_aabbs(long long(r) - l);
	AABB left_aabb;
	for (long long i = l; i < r; i++) {
		left_aabb.AddTriangle(vertices, triangles[i]);
		left_aabbs[i - l] = left_aabb;
	}
	int best_mid = -1;
	float best_sah_cost = FLT_MAX;
	// rigt aabbs
	AABB rigt_aabb;
	for (long long i = r; i > l; i--) {
		rigt_aabb.AddTriangle(vertices, triangles[i]);
		const float sah_cost = (i - l) * surface_area(left_aabbs[i - 1]) + (r - i + 1) * surface_area(rigt_aabb);
		if (sah_cost < best_sah_cost) {
			best_sah_cost = sah_cost;
			best_mid = i - 1;
		}
	}
	assert(best_mid != -1);
	return best_mid;
}