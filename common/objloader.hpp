#ifndef OBJLOADER_H
#define OBJLOADER_H
#include<iostream>
bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::ivec3>& out_vertex_ids,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::ivec3>& out_uv_ids,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::ivec3>& out_normal_ids
);

#endif