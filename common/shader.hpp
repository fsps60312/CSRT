#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include<common/control.hpp>
#include <common/gl_check_error.hpp>

class Shader {

private:
	GLuint program = 0;
	int layout_num = 0;
	std::string ReadFile(const std::string& file_path);
	std::string InjectComputeShaderCode(const std::string& code);
	GLuint CompileShader(const std::string& shader_code, GLenum shader_type);
	GLuint LoadAndCompileShader(const char* shader_file_path, GLenum shader_type);
	void LinkProgram(const GLuint program_ID, const GLuint shader_ID);
	GLuint LoadAndCompileShaders(const char* vertex_file_path, const char* fragment_file_path);
public:
	void Load(char v[], char f[], int l);
	void Load(char c[]);
	GLuint GetVariable(char name[])const;
	GLuint GetProgram();
	void Use()const;
	void Use(GLuint x, GLuint y, GLuint z)const;
	void Disable();
	void Delete();
};