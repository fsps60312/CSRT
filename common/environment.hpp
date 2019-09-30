#pragma once
#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include<common/gl_check_error.hpp>
#include<common/shader.hpp>
#include<common/control.hpp>

#include<cstdio>
#include<cstdlib>
#include<iostream>

namespace environment {
//private:
	void InitGLFW();
	void InitGLEW();
	void InitWindow();
	void GLShowInfo();
	void LoadShaders();
	void DrawSubWindow(GLint x, GLint y, GLsizei w, GLsizei h, Shader& texture_shader);
//public:
	void Init();
	bool IsKeyDown(int key);
	void DispatchShaders();
	glm::dvec2 GetCursorPos();
	void SetCursorPos(const glm::dvec2& pos);
	bool IsWindowClosed();
	Shader& GetTextureShader();
	Shader& GetComputeShader();
};
#endif
