#pragma once

#include <common/buffer_system.hpp>
#include<common/pod/pod.hpp>
#include<common/block/blocks.hpp>

BufferSystem::BufferSystem(){}

BufferSystem::BufferSystem(std::string filename)
{
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::ivec3>vertex_ids, uv_ids, normal_ids;
		loadOBJ(filename.c_str(), vertices, vertex_ids, uvs, uv_ids, normals, normal_ids);
		std::vector<Triangle>triangles;
		for (int i = 0; i < (int)vertex_ids.size(); i++)
			triangles.push_back(Triangle(glm::mat3(
				vertices[vertex_ids[i].x],
				vertices[vertex_ids[i].y],
				vertices[vertex_ids[i].z]
			)));
		obj = new VisibleObject();
		auto pod = new pod::Pod();
		obj->children.insert(pod);
		obj->children.insert(&block::Blocks::instance);
		/*if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->Translate(glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}*/
		//std::clog << "verify result  = " << root->Verify() << std::endl;

		// Identify a vertex Buffer Object
		//glGenBuffers(1, &materialBuffer);
		glGenBuffers(1, &trianglesBuffer);
		glGenBuffers(1, &materialsBuffer);
		glGenBuffers(1, &bvhNodeBuffer);
		glGenBuffers(1, &bvhAabbBuffer);
		glGenBuffers(1, &bvhRangeBuffer);
	}
	std::clog << "sizeof(Triangle) = " << sizeof(Triangle) << std::endl;
	std::clog << "sizeof(glm::mat3) = " << sizeof(glm::mat3) << std::endl;
}

int BufferSystem::GetTriangleNum() { return (int)BVHNode::glob_triangles.size(); }

void BufferSystem::Send()
{
	//for (auto c : obj->children)c->Rotate(glm::vec3(0, 1, 0),float( glm::acos(-1) / 100));
	{
		BVHNode::DeleteTree(root);
		BVHNode::ClearVectors();
		obj->Build(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
		root = BVHNode::Build();
		const int v = root->Verify();
		assert(v == (int)BVHNode::glob_bvh_nodes.size());
		/*std::clog << "triangles.size = " << BVHNode::glob_triangles.size() << std::endl;
		std::clog << "bvh.size       = " << BVHNode::glob_bvh_nodes.size() << std::endl;
		std::clog << "traverse       = " << v << std::endl;*/
		//triangles = bvh.GetTriangles();
		/*obj->children[0]->Rotate(glm::vec3(0, 1, 0), glm::acos(-1) / 100);
		obj->children[1]->Rotate(glm::vec3(1, 0, 0), -glm::acos(-1) / 100);
		obj->children[2]->Rotate(glm::vec3(0, 0, 1), -glm::acos(-1) / 100);
		obj->Update();*/
		const std::vector<Triangle>& triangles = BVHNode::glob_triangles;
		const std::vector<Material>& materials = Material::glob_materials;
		const std::vector<glm::ivec3>&nodes = BVHNode::glob_bvh_nodes;
		const auto &aabbs_raw = BVHNode::glob_bvh_aabbs;
		std::vector<glm::mat2x3>aabbs;
		for (const auto& aabb : aabbs_raw)aabbs.push_back(glm::mat2x3(aabb.GetMn(), aabb.GetMx()));
		const std::vector<glm::ivec2> &ranges = BVHNode::glob_tri_ranges;
		//assert(materials.size() == triangles.size() && aabbs.size() == nodes.size() && ranges.size() == nodes.size());

		const auto& padded_triangles = Padded(triangles);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 4U * padded_triangles.size(), padded_triangles.data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.

		const auto& padded_materials = Padded(materials);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialsBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 4U * padded_materials.size(), padded_materials.data(), GL_DYNAMIC_COPY);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhNodeBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::ivec3) + 4U) * nodes.size(), Padded(nodes).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhAabbBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(glm::mat2x3) + 8U) * aabbs.size(), Padded(aabbs).data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhRangeBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec2) * ranges.size(), ranges.data(), GL_DYNAMIC_COPY); // Give vertices to OpenGL.
	}
	// 1st buffer : materials

	// 2nd buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, trianglesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, materialsBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvhNodeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvhAabbBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvhRangeBuffer);
}

std::vector<glm::vec4> BufferSystem::Padded(const std::vector<glm::vec3>&s)const {
	std::vector<glm::vec4>ret;
	for (const auto& v : s)ret.push_back(glm::vec4(v, 0.0f));
	return ret;
}
std::vector<glm::ivec4> BufferSystem::Padded(const std::vector<glm::ivec3>&s)const {
	std::vector<glm::ivec4>ret;
	for (const auto& v : s)ret.push_back(glm::ivec4(v, 0.0f));
	return ret;
}
std::vector<glm::mat2x4> BufferSystem::Padded(const std::vector<glm::mat2x3>&s)const {
	std::vector<glm::mat2x4>ret;
	for (const auto& v : s)ret.push_back(glm::mat2x3(glm::vec4(v[0], 0.0f), glm::vec4(v[1], 0.0f)));
	return ret;
}
std::vector<glm::mat3x4> BufferSystem::Padded(const std::vector<glm::mat3>&s)const {
	std::vector<glm::mat3x4>ret;
	for (const auto& v : s)ret.push_back(glm::mat3x4(glm::vec4(v[0], 0.0f), glm::vec4(v[1], 0.0f), glm::vec4(v[2], 0.0f)));
	return ret;
}
std::vector<uint32_t> BufferSystem::Padded(const std::vector<Triangle>&triangles)const {
	std::vector<uint32_t>ret;
	for (const auto& triangle : triangles) {
		const auto vertices = triangle.GetVertices();
		for (int i = 0; i < 3; i++) {
			ret.push_back(glm::floatBitsToUint(vertices[i].x));
			ret.push_back(glm::floatBitsToUint(vertices[i].y));
			ret.push_back(glm::floatBitsToUint(vertices[i].z));
			ret.push_back(0);
		}
		ret.push_back(triangle.material_id);
		ret.push_back(0);
		ret.push_back(0);
		ret.push_back(0);
	}
	return ret;
}
std::vector<uint32_t> BufferSystem::Padded(const std::vector<Material>& materials)const {
	std::vector<uint32_t>ret;
	for (const auto& material : materials) {
		ret.push_back(glm::floatBitsToUint(material.ambient.r));
		ret.push_back(glm::floatBitsToUint(material.ambient.g));
		ret.push_back(glm::floatBitsToUint(material.ambient.b));
		ret.push_back(0);

		ret.push_back(glm::floatBitsToUint(material.diffuse.r));
		ret.push_back(glm::floatBitsToUint(material.diffuse.g));
		ret.push_back(glm::floatBitsToUint(material.diffuse.b));
		ret.push_back(0);

		ret.push_back(glm::floatBitsToUint(material.specular.r));
		ret.push_back(glm::floatBitsToUint(material.specular.g));
		ret.push_back(glm::floatBitsToUint(material.specular.b));
		ret.push_back(glm::floatBitsToUint(material.specular_exp));

		ret.push_back(glm::floatBitsToUint(material.alpha));
		ret.push_back(0);
		ret.push_back(0);
		ret.push_back(0);
	}
	return ret;
}