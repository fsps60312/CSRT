#include <stdio.h>
#include <stdlib.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <common/gl_check_error.hpp>
#include <common/mylib.hpp>

#define NOW_MODEL "suzanne" //"suzanne" , "teapot" , "monster"

GLFWwindow* window;

void DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat m, GLuint t, Shader& texture_shader, GLuint vertexBuffer_window);

int main(void)
{
	glm::vec3 light(0.0f, 10.0f, 5.0f);
	GLfloat  window_mode = 1.0f;
	Shader texture_shader, compute_shader;
	std::map<std::string, Buffer> obj;
	GLfloat width = getWidth(), height = getHeight();
	// --------------------
	// Set Window
	// --------------------
	{
		// Initialise GLFW ( Graphics Library FrameWork )
		//glewExperimental = true; // Needed in core profile
		const int glfw_init_result = glfwInit(); glfw_check_error();
		if (glfw_init_result != GLFW_TRUE) {
			fprintf(stderr, "Failed to initialize GLFW\n");
			system("pause");
			exit(0);
		}

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); glfw_check_error();

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(width , height , "Compute_Shader_Ray_Tracing", NULL, NULL); glfw_check_error();
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			glfwTerminate(); glfw_check_error();
			system("pause");
			exit(0);
		}
		glfwMakeContextCurrent(window); glfw_check_error();

		// Initialize GLEW ( OpenGL Extension Wrangler Library )
		const GLenum glew_init_result = glewInit();
		if (glew_init_result != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			glfwTerminate(); glfw_check_error();
			system("pause");
			exit(0);
		}

		// Ensure we can capture the keyboard being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); glfw_check_error();
	}


	// Identify a Vertex Array Object
	GLuint VertexArray;
	glGenVertexArrays(1, &VertexArray); gl_check_error();
	glBindVertexArray(VertexArray); gl_check_error();

	// Load OBJ
	//obj["plane"] = Buffer("plane.obj");
	//obj["cube"] = Buffer("cube.obj");
	obj["suzanne"] = Buffer("suzanne.obj");
	//obj["teapot"] = Buffer("teapot.obj");
	//obj["monster"] = Buffer("monster.obj");
	

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
	GLuint  vertexBuffer_window;
	glGenBuffers(1, &vertexBuffer_window); gl_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_window); gl_check_error();
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_window_vertex_buffer_data), g_window_vertex_buffer_data, GL_STATIC_DRAW); gl_check_error();


	// --------------------
	// Load Compute Shader
	// --------------------
#if 1
	// Load Shader & Get a handle for uniform
	compute_shader.Load("MyComputeShader.glsl");

	// Identify Texture
	GLuint computeTexture;
	glGenTextures(1, &computeTexture); gl_check_error();

	glActiveTexture(GL_TEXTURE0); gl_check_error();
	glBindTexture(GL_TEXTURE_2D, computeTexture); gl_check_error();
	
	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); gl_check_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); gl_check_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); gl_check_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); gl_check_error();
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0); gl_check_error();
	glBindImageTexture(0, computeTexture, 0, GL_FLOAT, 0, GL_WRITE_ONLY, GL_RGBA16F); gl_check_error();
#endif


	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do {

		// --------------------
		// Measure Speed
		// --------------------
		{
			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
				// printf and reset
				printf("%f ms/frame\n", 1000.0 / double(nbFrames));
				nbFrames = 0;
				lastTime += 1.0;
			}
		}


		// --------------------
		// Change Value
		// --------------------
		{
			// FoV
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
				subFoV(1.0f);
			if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
				addFoV(1.0f);
			computeMatricesFromInputs();

			// Light Position
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
				light.z += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
				light.x += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
				light.y += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
				light.y -= 0.01f;
			if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
				light.x -= 0.01f;
			if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
				light.z -= 0.01f;

			// Window Mode
			if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
				window_mode = 1.0f;
			if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
				window_mode = 2.0f;
			if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
				window_mode = 3.0f;
			if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
				window_mode = 4.0f;
			if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
				window_mode = 5.0f;
			if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
				window_mode = 6.0f;
			if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)
				window_mode = 7.0f;
		}


		// --------------------
		// Pass Value
		// --------------------
		{
			// Texture Shader
			texture_shader.Use();
			glUniform1f(texture_shader.GetVariable("Mode"), window_mode);

			// Compute Shader
			compute_shader.Use();
			glUniform3f(compute_shader.GetVariable("eye"), getPosition().x, getPosition().y, getPosition().z);
			glUniform3f(compute_shader.GetVariable("view"), getDirection().x, getDirection().y, getDirection().z);
			glUniform3f(compute_shader.GetVariable("up"), getUp().x, getUp().y, getUp().z);
			glUniform3f(compute_shader.GetVariable("light"), light.x, light.y, light.z);
			glUniform1f(compute_shader.GetVariable("fov"), getFoV());
#if 1
			glUniform1i(compute_shader.GetVariable("tri_num"), obj[NOW_MODEL].GetTriangleNum());
			obj[NOW_MODEL].Send();
#else
			Draw_Scene_1();
#endif
		}


		// --------------------
		// Compute Shading
		// --------------------
		{
			compute_shader.Use(width / 4.0f, height / 4.0f, 1);

			// make sure writing to image has finished before read
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

			glBindImageTexture(0, computeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

			compute_shader.Disable();

			DrawSubWindow(0.0f, 0.0f, width, height, 1.0f, computeTexture, texture_shader, vertexBuffer_window);
		}


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// --------------------
	// Clean
	// --------------------
	{
		// Buffer & Obj & Model
		for (std::map<std::string, Buffer>::iterator it = obj.begin(); it != obj.end(); ++it)
			(it->second).Delete();

		// Texture Shader
		glDeleteBuffers(1, &vertexBuffer_window);
		texture_shader.Delete();
		
		// Compute Shader
		glDeleteTextures(1, &computeTexture);
		compute_shader.Delete();

		glDeleteVertexArrays(1, &VertexArray);
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat m, GLuint t, Shader& texture_shader,GLuint vertexBuffer_window)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(x, y, w, h);

	// Use shader
	texture_shader.Use();

	glUniform1f(texture_shader.GetVariable("Mode"), m);

	// Send Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t);
	glUniform1i(texture_shader.GetVariable("RenderedTexture"), 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_window);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/// Draw Rendered Texture
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); // Starting from vertex 0; 3 vertices total -> 1 triangle

	texture_shader.Disable();
}