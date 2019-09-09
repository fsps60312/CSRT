#pragma once

#include<common/shader.hpp>
#include<common/control.hpp>
#include<common/gl_check_error.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include<cstdio>
#include<cstdlib>
#include<iostream>

class Environment {
private:
	GLFWwindow *window;
	Shader texture_shader, compute_shader;
	GLuint compute_texture;
	GLuint window_vertex_buffer;
	void InitGLFW();
	void InitGLEW();
	void InitWindow();
	void GLShowInfo();
	void LoadShaders();
	void DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, Shader& texture_shader);
public:
	void Init();
	bool IsKeyDown(int key)const;
	void DispatchShaders();
	glm::dvec2 GetCursorPos();
	void SetCursorPos(const glm::dvec2& pos);
	bool IsWindowClosed() { return glfwWindowShouldClose(window) != 0; }
	Shader& GetTextureShader() { return texture_shader; }
	Shader& GetComputeShader() { return compute_shader; }
};