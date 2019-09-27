#pragma once

#include <common/buffer_system.hpp>
#include<common/pod/pod.hpp>

BufferSystem::BufferSystem(){}

BufferSystem::BufferSystem(std::string filename)
{
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::ivec3>vertex_ids, uv_ids, normal_ids;
		loadOBJ(filename.c_str(), vertices, vertex_ids, uvs, uv_ids, normals, normal_ids);
		std::vector<glm::mat3>triangles;
		for (int i = 0; i < (int)vertex_ids.size(); i++)
			triangles.push_back(glm::mat3(
				vertices[vertex_ids[i].x],
				vertices[vertex_ids[i].y],
				vertices[vertex_ids[i].z]
			));
		obj = new VisibleObject();
		auto pod = new pod::Pod();
		pod->Translate(glm::vec3(0, 0, -2));
		obj->children.push_back((VisibleObject*)pod);
		/*obj->children.push_back(new VisibleObject(triangles));
		obj->children.push_back(new VisibleObject(triangles));
		obj->children.push_back(new VisibleObject(triangles));*/
		if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->Translate(glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}
		//std::clog << "verify result  = " << root->Verify() << std::endl;

		// Identify a vertex Buffer Object
		glGenBuffers(1, &materialBuffer);
		glGenBuffers(1, &trianglesBuffer);
		glGenBuffers(1, &bvhNodeBuffer);
		glGenBuffers(1, &bvhAabbBuffer);
		glGenBuffers(1, &bvhRangeBuffer);
	}
}

int BufferSystem::GetTriangleNum() { return (int)BVHNode::glob_triangles.size(); }

void BufferSystem::Send()
{
	for (auto c : obj->children)c->Rotate(glm::vec3(0, 1, 0), glm::acos(-1) / 100);
	{
		BVHNode::DeleteTree(root);
		BVHNode::ClearVectors();
		obj->Build(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
		root = BVHNode::Build();
		const int v = root->Verify();
		assert(v == (int)BVHNode::glob_bvh_nodes.size() && BVHNode::glob_triangles.size() == 12);
		std::clog << "triangles.size = " << BVHNode::glob_triangles.size() << std::endl;
		std::clog << "bvh.size       = " << BVHNode::glob_bvh_nodes.size() << std::endl;
		std::clog << "traverse       = " << v << std::endl;
		//triangles = bvh.GetTriangles();
		/*obj->children[0]->Rotate(glm::vec3(0, 1, 0), glm::acos(-1) / 100);
		obj->children[1]->Rotate(glm::vec3(1, 0, 0), -glm::acos(-1) / 100);
		obj->children[2]->Rotate(glm::vec3(0, 0, 1), -glm::acos(-1) / 100);
		obj->Update();*/
		std::vector<glm::mat3>triangles = BVHNode::glob_triangles;
		materials = std::vector<int>(GetTriangleNum(), 1);

		//const std::vector<glm::ivec3>nodes = bvh.GetNodes();
		const std::vector<glm::ivec3>nodes = BVHNode::glob_bvh_nodes;
		//const auto aabbs_raw = bvh.GetAabbs();
		const auto aabbs_raw = BVHNode::glob_bvh_aabbs;
		std::vector<glm::mat2x3>aabbs;
		for (const auto& aabb : aabbs_raw)aabbs.push_back(glm::mat2x3(aabb.GetMn(), aabb.GetMx()));
		//std::clog << aabbs[1][0].x << " " << aabbs[1][0].y << " " << aabbs[1][0].z << ", " << aabbs[1][1].x << " " << aabbs[1][1].y << " " << aabbs[1][1].z << std::endl;
		//const std::vector<glm::ivec2> ranges = bvh.GetRanges();
		const std::vector<glm::ivec2> ranges = BVHNode::glob_tri_ranges;
		assert(materials.size() == triangles.size() && aabbs.size() == nodes.size() && ranges.size() == nodes.size());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * materials.size(), materials.data(), GL_DYNAMIC_COPY); // Give id_materials to OpenGL.
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::mat3) + 12U) * triangles.size(), Padded(triangles).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhNodeBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::ivec3) + 4U) * nodes.size(), Padded(nodes).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhAabbBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::mat2x3) + 8U) * aabbs.size(), Padded(aabbs).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhRangeBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec2) * ranges.size(), ranges.data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	}
	// 1st buffer : materials
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, materialBuffer);

	// 2nd buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, trianglesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvhNodeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvhAabbBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvhRangeBuffer);
}

std::vector<glm::vec4> BufferSystem::Padded(const std::vector<glm::vec3>s)const {
	std::vector<glm::vec4>ret;
	for (const auto& v : s)ret.push_back(glm::vec4(v, 0.0f));
	return ret;
}
std::vector<glm::ivec4> BufferSystem::Padded(const std::vector<glm::ivec3>s)const {
	std::vector<glm::ivec4>ret;
	for (const auto& v : s)ret.push_back(glm::ivec4(v, 0.0f));
	return ret;
}
std::vector<glm::mat2x4> BufferSystem::Padded(const std::vector<glm::mat2x3>s)const {
	std::vector<glm::mat2x4>ret;
	for (const auto& v : s)ret.push_back(glm::mat2x3(glm::vec4(v[0], 0.0f), glm::vec4(v[1], 0.0f)));
	return ret;
}
std::vector<glm::mat3x4> BufferSystem::Padded(const std::vector<glm::mat3>s)const {
	std::vector<glm::mat3x4>ret;
	for (const auto& v : s)ret.push_back(glm::mat3x4(glm::vec4(v[0], 0.0f), glm::vec4(v[1], 0.0f), glm::vec4(v[2], 0.0f)));
	return ret;
}