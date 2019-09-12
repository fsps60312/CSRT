#pragma once

#include <common/buffer_system.hpp>

BufferSystem::BufferSystem(std::string filename)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(filename.c_str(), vertices, uvs, normals);

	tri_num = vertices.size() / 3;
	std::vector<int> materials(tri_num, 1);

	// Identify a vertex Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::vec3)+4U) * vertices.size(), Padded(vertices).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.

	// Identify a vertex Buffer Object
	glGenBuffers(1, &materialBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * materials.size(), &materials[0], GL_DYNAMIC_COPY); // Give id_materials to OpenGL.
}

void BufferSystem::Send()
{
	// 1st buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuffer);
	
	// 2nd buffer : materials
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialBuffer);
}

std::vector<glm::vec4> BufferSystem::Padded(const std::vector<glm::vec3>s)const {
	std::vector<glm::vec4>ret;
	for (const auto& v : s)ret.push_back(glm::vec4(v, 0.0f));
	return ret;
}