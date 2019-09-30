#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<common/objloader.hpp>
#include<common/visible_object.hpp>
#include<common/control.hpp>



// Delete after Assimp finish
class BufferSystem {

private:
	BVHNode* root = NULL;
	GLuint trianglesBuffer = 0;
	GLuint bvhNodeBuffer = 0;
	GLuint bvhAabbBuffer = 0;
	GLuint bvhRangeBuffer = 0;
	std::vector<glm::vec4> Padded(const std::vector<glm::vec3>s)const;
	std::vector<glm::ivec4> Padded(const std::vector<glm::ivec3>s)const;
	std::vector<glm::mat2x4> Padded(const std::vector<glm::mat2x3>s)const;
	std::vector<glm::mat3x4> Padded(const std::vector<glm::mat3>s)const;
	std::vector<uint32_t> Padded(const std::vector<Triangle>s)const;
public:
	VisibleObject* obj = NULL;
	BufferSystem();
	BufferSystem(std::string filename);
	void Add(){
		std::string filename = NOW_MODEL + std::string(".obj");
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
		if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->TranslatePrepend(-glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}
		obj->children.push_back(new VisibleObject(triangles));
		if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->TranslatePrepend(glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}
	}
	void Remove() {
		if (obj->children.size()<=0)return;
		if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->TranslatePrepend(-glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}
		obj->children.pop_back();
		if (obj->children.size() > 1)for (int i = 0; i < (int)obj->children.size(); i++) {
			const float dx = 3, dz = -3;
			obj->children[i]->TranslatePrepend(glm::vec3(-dx + 2 * dx * i / (obj->children.size() - 1), 0, dz));
		}
	}
	void Send();
	int GetTriangleNum();
};

// Not yet !
/*
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <SOIL.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
private:
	GLuint VAO = 0, VBO = 0, EBO = 0;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
	};
	void Set();
	void Draw(Shader shader);
};

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model() {};
	Model(GLchar* path) {
		this->loadModel(path);
	}
	void Set() {
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Set();
	}
	void Draw(Shader shader) {
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}
};

GLuint TextureFromFile(const char* path, std::string directory);
*/