#pragma once

#include <common/buffer_system.hpp>

BufferSystem::BufferSystem(std::string filename)
{
	loadOBJ(filename.c_str(), vertices, vertex_ids, uvs, uv_ids, normals, normal_ids);

	bvh = BVH(vertex_ids);
	bvh.Build(vertices, new VisibleObject(0, vertex_ids.size() - 1)); // TODO

	// Identify a vertex Buffer Object
	glGenBuffers(1, &materialBuffer);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &vertexIdBuffer);
	glGenBuffers(1, &bvhNodeBuffer);
	glGenBuffers(1, &bvhAabbBuffer);
	glGenBuffers(1, &bvhRangeBuffer);
}

void BufferSystem::Send()
{
	vertex_ids = bvh.GetTriangles();
	tri_num = vertex_ids.size();
	materials = std::vector<int>(tri_num, 1);

	const std::vector<glm::ivec3>nodes = bvh.GetNodes();
	const auto aabbs_raw = bvh.GetAabbs();
	std::vector<glm::mat2x3>aabbs;
	for (const auto& aabb : aabbs_raw)aabbs.push_back(glm::mat2x3(aabb.GetMn(), aabb.GetMx()));
	//std::clog << aabbs[1][0].x << " " << aabbs[1][0].y << " " << aabbs[1][0].z << ", " << aabbs[1][1].x << " " << aabbs[1][1].y << " " << aabbs[1][1].z << std::endl;
	const std::vector<glm::ivec2> ranges = bvh.GetRanges();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * materials.size(), materials.data(), GL_DYNAMIC_COPY); // Give id_materials to OpenGL.
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::vec3) + 4U) * vertices.size(), Padded(vertices).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexIdBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::ivec3) + 4U) * vertex_ids.size(), Padded(vertex_ids).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhNodeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::ivec3) + 4U) * nodes.size(), Padded(nodes).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhAabbBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::mat2x3) + 8U) * aabbs.size(), Padded(aabbs).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhRangeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec2) * ranges.size(), ranges.data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	// 1st buffer : materials
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, materialBuffer);

	// 2nd buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vertexIdBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvhNodeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvhAabbBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, bvhRangeBuffer);
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