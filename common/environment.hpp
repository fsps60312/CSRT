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
	void InitWindow();
	void InitGLEW();
	void InitFBO();
	void GLShowInfo();
//public:
	void Init();
	void DrawSubWindow(GLint x, GLint y, GLsizei w, GLsizei h, Shader& texture_shader, GLuint compute_texture);
	bool IsKeyDown(int key);
	glm::dvec2 GetCursorPos();
	void SetCursorPos(const glm::dvec2& pos);
	bool IsWindowClosed();
};
#endif
