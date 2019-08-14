#pragma once
std::string ReadFile(const std::string& file_path);
GLuint CompileShader(const std::string& shader_code, GLenum shader_type);
GLuint LoadAndCompileShader(const char* shader_file_path, GLenum shader_type);
void LinkProgram(const GLuint program_ID, const GLuint shader_ID);
GLuint LoadAndCompileShaders(const char* vertex_file_path, const char* fragment_file_path);