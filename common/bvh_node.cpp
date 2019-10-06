#include<common/bvh_node.hpp>
std::mutex BVHNode::new_node_lock;
std::vector<glm::ivec3>BVHNode::glob_bvh_nodes;
std::vector<AABB>BVHNode::glob_bvh_aabbs;
std::vector<glm::ivec2>BVHNode::glob_tri_ranges;
std::vector<Triangle>BVHNode::glob_triangles;
void BVHNode::ClearVectors() {
	glob_bvh_nodes.clear();
	glob_bvh_aabbs.clear();
	glob_tri_ranges.clear();
	glob_triangles.clear();
}
void BVHNode::DeleteTree(BVHNode* root) {
	if (root == NULL)return;
	DeleteTree(root->l);
	DeleteTree(root->r);
	delete root;
}

int BVHNode::GetId(BVHNode* o) { return o ? o->id : -1; }
void BVHNode::SetL(BVHNode* l) { this->l = l; glob_bvh_nodes[id].x = GetId(l); }
void BVHNode::SetR(BVHNode* r) { this->r = r; glob_bvh_nodes[id].y = GetId(r); }
void BVHNode::SetRangeL() { glob_tri_ranges[id].x = (int)glob_triangles.size(); }
void BVHNode::SetRangeR() { glob_tri_ranges[id].y = (int)glob_triangles.size() - 1; }

int BVHNode::Verify() {
	if (l == NULL && r == NULL) {
		return 1;
	} else if (l != NULL && r != NULL) {
		assert(glob_bvh_nodes[l->id].z == id && glob_bvh_nodes[id].x == l->id);
		assert(glob_bvh_nodes[r->id].z == id && glob_bvh_nodes[id].y == r->id);
		assert(glob_tri_ranges[l->id].x == glob_tri_ranges[id].x);
		assert(glob_tri_ranges[l->id].y + 1 == glob_tri_ranges[r->id].x);
		assert(glob_tri_ranges[r->id].y == glob_tri_ranges[id].y);
		return 1 + l->Verify() + r->Verify();
	} else if (l != NULL && r == NULL) {
		assert(glob_bvh_nodes[l->id].z == id && glob_bvh_nodes[id].x == l->id);
		assert(glob_bvh_nodes[id].y == -1);
		assert(glob_tri_ranges[l->id].x == glob_tri_ranges[id].x);
		assert(glob_tri_ranges[l->id].y == glob_tri_ranges[id].y);
		return 1 + l->Verify();
	} else {
		assert(l == NULL && r != NULL);
		assert(glob_bvh_nodes[id].x == -1);
		assert(glob_bvh_nodes[r->id].z == id && glob_bvh_nodes[id].y == r->id);
		assert(glob_tri_ranges[r->id].x == glob_tri_ranges[id].x);
		assert(glob_tri_ranges[r->id].y == glob_tri_ranges[id].y);
		return 1 + r->Verify();
	}
}

void BVHNode::NewNode() {
	glob_bvh_nodes.push_back(glm::ivec3(GetId(l), GetId(r), GetId(p)));
	glob_bvh_aabbs.push_back(AABB());
	glob_tri_ranges.push_back(glm::ivec2(-1));
}

BVHNode::BVHNode(BVHNode* parent) :id((int)glob_bvh_nodes.size()), p(parent) {
	NewNode();
}

void BVHNode::Build(const int l, const int r, std::vector<int>& tri_ids) {
	assert(0 <= l && l <= r && r < (int)glob_triangles.size());
	// range
	glob_tri_ranges[id] = glm::ivec2(l, r);
	// cal aabb
	auto& aabb = glob_bvh_aabbs[id] = AABB();
	for (int i = l; i <= r; i++)aabb.AddTriangle(glob_triangles[tri_ids[i]]);
	if (r - l + 1 <= 2) {
		return;
	}
	// split
	const int mid = CalMid(id, tri_ids);
	assert(l <= mid && mid < r);
	// dfs
	new_node_lock.lock();
	BVHNode* lch = new BVHNode(this), * rch = new BVHNode(this);
	new_node_lock.unlock();
	if (r - l + 1 > 10000) {
		std::thread l_thread = std::thread([lch, l, mid, &tri_ids]() {
			lch->Build(l, mid, tri_ids);
		});
		std::thread r_thread = std::thread([rch, mid, r, &tri_ids]() {
			rch->Build(mid + 1, r, tri_ids);
		});
		l_thread.join();
		r_thread.join();
	} else {
		lch->Build(l, mid, tri_ids);
		rch->Build(mid + 1, r, tri_ids);
	}
	SetL(lch);
	SetR(rch);
}
int BVHNode::CalMid(const int id, std::vector<int>& tri_ids) {
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
	std::sort(tri_ids.begin() + l, tri_ids.begin() + r + 1, [&c, &dot_max](const int t1, const int t2)->bool {
	//std::nth_element(tri_ids.begin() + l, tri_ids.begin() + (l + r) / 2, tri_ids.begin() + r + 1, [&c, &dot_max](const int t1, const int t2)->bool {
		return dot_max(glob_triangles[t1].GetVertices(), c) < dot_max(glob_triangles[t2].GetVertices(), c);
	});
	// left aabbs
	std::vector<AABB>left_aabbs(long long(r) - l);
	AABB left_aabb;
	for (long long i = l; i < r; i++) {
		left_aabb.AddTriangle(glob_triangles[tri_ids[i]]);
		left_aabbs[i - l] = left_aabb;
	}
	int best_mid = -1;
	float best_sah_cost = FLT_MAX;
	// rigt aabbs
	AABB rigt_aabb;
	for (long long i = r; i > l; i--) {
		rigt_aabb.AddTriangle(glob_triangles[tri_ids[i]]);
		const float sah_cost = (i - l) * surface_area(left_aabbs[i - l - 1]) + (r - i + 1) * surface_area(rigt_aabb);
		if (sah_cost < best_sah_cost) {
			best_sah_cost = sah_cost;
			best_mid = (int)(i - 1);
		}
	}
	assert(best_mid != -1);
	return best_mid;
}

BVHNode* BVHNode::Build() {
	BVHNode* o = new BVHNode(NULL);
	std::vector<int>tri_ids(glob_triangles.size());
	for (int i = 0; i < (int)glob_triangles.size(); i++)tri_ids[i] = i;
	o->Build(0, (int)(glob_triangles.size() - 1), tri_ids);
	std::vector<Triangle>new_glob_triangles;
	for (int i = 0; i < (int)glob_triangles.size(); i++)new_glob_triangles.push_back(glob_triangles[tri_ids[i]]);
	glob_triangles.swap(new_glob_triangles);
	return o;
}