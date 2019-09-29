#include<common/environment.hpp>
bool Environment::IsWindowClosed() { return glfwWindowShouldClose(window) != 0; }
Shader& Environment::GetTextureShader() { return texture_shader; }
Shader& Environment::GetComputeShader() { return compute_shader; }
bool Environment::IsKeyDown(int key)const {
	return glfwGetKey(window, key) == GLFW_PRESS;
}
glm::dvec2 Environment::GetCursorPos() {
	glm::dvec2 ret;
	glfwGetCursorPos(window, &ret.x, &ret.y);
	return ret;
}
void Environment::SetCursorPos(const glm::dvec2 &pos) {
	glfwSetCursorPos(window, pos.x, pos.y);
}
void Environment::Init() {
	InitGLFW();
	InitWindow();
	InitGLEW();
	GLShowInfo();
	LoadShaders();
}
void Environment::DispatchShaders() {
	// --------------------
	// Compute Shading
	// --------------------
	{
		// make sure writing to image has finished before read
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

		compute_shader.Use(WIDTH / WORK_GROUP_SIZE_X, HEIGHT / WORK_GROUP_SIZE_Y, 1);

		glBindImageTexture(0, compute_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		compute_shader.Disable();

		DrawSubWindow(0.0f, 0.0f, WIDTH, HEIGHT, GetTextureShader());
	}


	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();

}
void Environment::DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, Shader& texture_shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(x, y, w, h);

	// Use shader
	texture_shader.Use();

	// Send Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, compute_texture);
	glUniform1i(texture_shader.GetVariable("RenderedTexture"), 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, window_vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/// Draw Rendered Texture
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); // Starting from vertex 0; 3 vertices total -> 1 triangle

	texture_shader.Disable();
}
void Environment::LoadShaders() {
	// --------------------
	// Load Texture Shader
	// --------------------
	// Load Shader & Get a handle for uniform
	texture_shader.Load("MyTextureVertexShader.glsl", "MyTextureFragmentShader.glsl", 1);

	// The fullscreen FBO
	static const GLfloat g_window_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};

	// Identify a vertex Buffer Object
	glGenBuffers(1, &window_vertex_buffer); gl_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, window_vertex_buffer); gl_check_error();
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_window_vertex_buffer_data), g_window_vertex_buffer_data, GL_STATIC_DRAW); gl_check_error();


	// --------------------
	// Load Compute Shader
	// --------------------
	// Load Shader & Get a handle for uniform
	compute_shader.Load("MyComputeShader.glsl");

	// Identify Texture
	glGenTextures(1, &compute_texture); gl_check_error();

	glActiveTexture(GL_TEXTURE0); gl_check_error();
	glBindTexture(GL_TEXTURE_2D, compute_texture); gl_check_error();

	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); gl_check_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); gl_check_error();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, 0); gl_check_error();
	glBindImageTexture(0, compute_texture, 0, GL_FLOAT, 0, GL_WRITE_ONLY, GL_RGBA16F); gl_check_error();
}
void Environment::GLShowInfo() {
	GLint64 v0, v1, v2;
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &v0); gl_check_error();
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &v1); gl_check_error();
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &v2); gl_check_error();
	std::clog << "GL_MAX_COMPUTE_WORK_GROUP_COUNT    " << v0 << " " << v1 << " " << v2 << std::endl;
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &v0); gl_check_error();
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &v1); gl_check_error();
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &v2); gl_check_error();
	std::clog << "GL_MAX_COMPUTE_WORK_GROUP_SIZE     " << v0 << " " << v1 << " " << v2 << std::endl;
}
void Environment::InitGLEW() {
	// Initialize GLEW ( OpenGL Extension Wrangler Library )
	const GLenum glew_init_result = glewInit();
	if (glew_init_result != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate(); glfw_check_error();
		system("pause");
		exit(0);
	}
}
void Environment::InitGLFW() {
	// Initialise GLFW ( Graphics Library FrameWork )
	const int glfw_init_result = glfwInit(); glfw_check_error();
	if (glfw_init_result != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("pause");
		exit(0);
	}
}
void Environment::InitWindow() {
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); glfw_check_error();

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Compute_Shader_Ray_Tracing", NULL, NULL); glfw_check_error();
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate(); glfw_check_error();
		system("pause");
		exit(0);
	}
	glfwMakeContextCurrent(window); glfw_check_error();

	// Ensure we can capture the keyboard being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); glfw_check_error();
}