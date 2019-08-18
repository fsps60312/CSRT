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



// Delete after Assimp finish
class Buffer {

private:
	int tri_num = 0;
	GLuint vertexBuffer = 0;
	GLuint materialBuffer = 0; // index
	int mode = 0;
public:
	Buffer() {};
	Buffer(std::string filename);
	void Send();
	int GetTriangleNum() { return tri_num; };
	void Delete() {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &materialBuffer);
	}
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