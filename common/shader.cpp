#include <common/shader.hpp>

#pragma region Shader
GLuint Shader::GetVariable(char name[]) const{ return glGetUniformLocation(program, name); }
GLuint Shader::GetProgram() { return program; }
void Shader::Use()const { glUseProgram(program); }
void Shader::Use(GLuint x, GLuint y, GLuint z)const {
	glUseProgram(program);
	glDispatchCompute(x, y, z);
}
void Shader::Disable() {
	for (int i = 0; i < layout_num; ++i)
		glDisableVertexAttribArray(i);
}
void Shader::Delete() { glDeleteProgram(program); }
void Shader::Load(char v[], char f[], int l) {
	program = LoadAndCompileShaders(v, f);
	layout_num = l;
}
void Shader::Load(char c[]) {
	program = LoadAndCompileShader(c, GL_COMPUTE_SHADER);
	layout_num = 0;
}

std::string Shader::ReadFile(const std::string& file_path) {
	std::ifstream file_stream(file_path, std::ios::in);
	if (!file_stream.is_open()) {
		fprintf(stderr, "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path.c_str());
		system("pause"); exit(0);
	}
	// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	return std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
}
GLuint Shader::LoadAndCompileShaders(const char* vertex_file_path, const char* fragment_file_path) {
	const std::string vertex_code = ReadFile(vertex_file_path), fragment_code = ReadFile(fragment_file_path);
	fprintf(stderr, "Compiling vertex shader: %s\n", vertex_file_path);
	const GLuint vertex_ID = CompileShader(vertex_code, GL_VERTEX_SHADER);
	fprintf(stderr, "Compiling fragment shader: %s\n", fragment_file_path);
	const GLuint fragment_ID = CompileShader(fragment_code, GL_FRAGMENT_SHADER);
	const GLuint program_ID = glCreateProgram();
	if (program_ID == 0) {
		fprintf(stderr, "Failed to create program.\n");
		system("pause"); exit(0);
	}
	glAttachShader(program_ID, vertex_ID);
	gl_check_error();
	glAttachShader(program_ID, fragment_ID);
	gl_check_error();
	LinkProgram(program_ID, fragment_ID);
	glDetachShader(program_ID, vertex_ID);
	gl_check_error();
	glDetachShader(program_ID, fragment_ID);
	gl_check_error();
	glDeleteShader(vertex_ID);
	gl_check_error();
	glDeleteShader(fragment_ID);
	gl_check_error();
	return program_ID;
}

std::string Shader::InjectComputeShaderCode(const std::string& code) {
	std::string ans;
	int cur = 0;
	for (; cur < (int)code.size(); cur++) {
		ans += code[cur];
		if (code[cur] == '\n')break;
	}
	char inject[100];
	sprintf(inject, "layout(local_size_x = %d, local_size_y = %d) in;", WORK_GROUP_SIZE_X, WORK_GROUP_SIZE_Y);
	std::clog << "injected: " << inject << std::endl;
	ans += inject;
	for (; cur < (int)code.size(); cur++)ans += code[cur];
	return ans;
}

GLuint Shader::LoadAndCompileShader(const char* shader_file_path,GLenum shader_type) {
	const std::string shader_code = InjectComputeShaderCode(ReadFile(shader_file_path));
	fprintf(stderr, "Compiling shader: %s\n", shader_file_path);
	const GLuint shader_ID = CompileShader(shader_code, shader_type);
	const GLuint program_ID = glCreateProgram();
	if (program_ID == 0) {
		fprintf(stderr, "Failed to create program.\n");
		system("pause"); exit(0);
	}
	glAttachShader(program_ID, shader_ID);
	gl_check_error();
	LinkProgram(program_ID, shader_ID);
	glDetachShader(program_ID, shader_ID);
	gl_check_error();
	glDeleteShader(shader_ID);
	gl_check_error();
	return program_ID;
}

void Shader::LinkProgram(const GLuint program_ID,const GLuint shader_ID) {
	glLinkProgram(program_ID);
	gl_check_error();
	GLsizei program_info_log_length;
	static GLchar program_info_log[1024];
	glGetProgramInfoLog(program_ID, 1024, &program_info_log_length, program_info_log);
	gl_check_error();
	if (program_info_log_length > 0) {
		fprintf(stderr, "Program Info Log (%u):\n%s\n", program_info_log_length, program_info_log);
		system("pause"); exit(0);
	}
}

GLuint Shader::CompileShader(const std::string& shader_code,const GLenum shader_type) {
	GLuint shader_ID = glCreateShader(shader_type);
	gl_check_error();

	const GLchar* shader_code_pointer = (GLchar*)shader_code.c_str();
	const GLint shader_code_length = (GLint)shader_code.length();

	glShaderSource(shader_ID, 1, &shader_code_pointer, &shader_code_length);
	gl_check_error();

	glCompileShader(shader_ID);
	gl_check_error();

	GLint shader_compile_result;
	GLsizei shader_info_log_length;
	static GLchar shader_info_log[1024];
	glGetShaderInfoLog(shader_ID, 1024, &shader_info_log_length, shader_info_log);
	gl_check_error();
	if (shader_info_log_length > 0) {
		fprintf(stderr, "Shader Info Log (%u):\n%s\n", shader_info_log_length, shader_info_log);
		system("pause");
	}
	glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &shader_compile_result);
	if (shader_compile_result != GL_TRUE) {
		fprintf(stderr, "Compile Error.\n");
		system("pause"); exit(0);
	}
	return shader_ID;
}
#pragma endregion