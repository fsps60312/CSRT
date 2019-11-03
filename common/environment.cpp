#include<common/environment.hpp>
namespace environment {
	GLFWwindow* window;
	GLuint window_vertex_buffer;
	bool IsWindowClosed() { return glfwWindowShouldClose(window) != 0; }
	bool IsKeyDown(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
	glm::dvec2 GetCursorPos() {
		glm::dvec2 ret;
		glfwGetCursorPos(window, &ret.x, &ret.y);
		return ret;
	}
	void SetCursorPos(const glm::dvec2& pos) {
		glfwSetCursorPos(window, pos.x, pos.y);
	}
	void Init() {
		InitGLFW();
		InitWindow();
		InitGLEW();
		GLShowInfo();
	}
	void DrawSubWindow(GLint x, GLint y, GLsizei w, GLsizei h, Shader& texture_shader, GLuint compute_texture)
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
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	void GLShowInfo() {
		GLint64 v0, v1, v2;
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &v0); gl_check_error();
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &v1); gl_check_error();
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &v2); gl_check_error();
		std::clog << "GL_MAX_COMPUTE_WORK_GROUP_COUNT    " << v0 << " " << v1 << " " << v2 << std::endl;
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &v0); gl_check_error();
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &v1); gl_check_error();
		glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &v2); gl_check_error();
		std::clog << "GL_MAX_COMPUTE_WORK_GROUP_SIZE     " << v0 << " " << v1 << " " << v2 << std::endl;
		glGetInteger64v(GL_MAJOR_VERSION, &v0); gl_check_error();
		glGetInteger64v(GL_MINOR_VERSION, &v1); gl_check_error();
		std::clog << "GL_VERSION     " << v0 << "." << v1 << std::endl;
	}
	void InitGLEW() {
		// Initialize GLEW ( OpenGL Extension Wrangler Library )
		const GLenum glew_init_result = glewInit();
		if (glew_init_result != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			glfwTerminate(); glfw_check_error();
			system("pause");
			exit(0);
		}
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
	}
	void InitGLFW() {
		// Initialise GLFW ( Graphics Library FrameWork )
		const int glfw_init_result = glfwInit(); glfw_check_error();
		if (glfw_init_result != GLFW_TRUE) {
			fprintf(stderr, "Failed to initialize GLFW\n");
			system("pause");
			exit(0);
		}
	}
	void InitWindow() {

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); glfw_check_error();

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(SCREEN_WIDTH * SCREEN_SCALE_X, SCREEN_HEIGHT * SCREEN_SCALE_Y, "Compute_Shader_Ray_Tracing", NULL, NULL); glfw_check_error();
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
}