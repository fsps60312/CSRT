#pragma once

#include <common/buffer_system.hpp>

BufferSystem::BufferSystem(std::string filename)
{
	std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;
	loadOBJ(filename.c_str(), vertices, uvs, normals);

	tri_num = vertices.size() / 9;
	std::vector<int> materials(tri_num, 1);

	// Identify a vertex Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_DYNAMIC_COPY); // Give vertices to OpenGL.

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
