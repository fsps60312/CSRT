#include <string>
#include <iostream>

#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include "gl_check_error.hpp"

void glfw_check_error(bool halt) {
	//int code = glfwGetError(NULL);
	//if (code != GLFW_NO_ERROR)return;
		//handle_error(code);
}
void gl_check_error(bool halt) {
	auto gl_error_string = [](GLenum const err)->std::string {
		switch (err)
		{
			// opengl 2 errors (8)
		case GL_NO_ERROR:
			return "GL_NO_ERROR";
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		case GL_TABLE_TOO_LARGE:
			return "GL_TABLE_TOO_LARGE";
			// opengl 3 errors (1)
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
			// gles 2, 3 and gl 4 error are handled by the switch above
		default:
			return "UNKNOWN_ERROR";
		}
	};
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::clog << gl_error_string(error) << std::endl;
		system("pause");
		if (halt)exit(0);
	}
}