#include <stdio.h>
#include <stdlib.h>

#include <common/mylib.hpp>

#define NOW_MODEL "suzanne" //"suzanne" , "teapot" , "monster"
#define WIDTH_SCALE  1.0f
#define HEIGHT_SCALE 1.0f

glm::vec3 light(0.0f, 10.0f, 5.0f);
GLfloat borderColor[]  = { 1.0f, 1.0f, 1.0f, 1.0f }, window_mode = 1.0f;
GLenum  DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

Shader texture_shader, compute_shader;
std::map<std::string, Buffer> obj;
std::map<std::string, GLuint> texture;

GLFWwindow* window;
GLfloat width = getWidth(), height = getHeight();
GLuint  vertexBuffer_window = 0;

void DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat m, GLuint t);
void TtoT(GLuint f, GLuint t, GLfloat m, GLuint b);
void Draw_Scene_1();

int main(void)
{
	// --------------------
	// Set Window
	// --------------------
	{
		// Initialise GLFW ( Graphics Library FrameWork )
		glewExperimental = true; // Needed in core profile
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.X
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // We want OpenGL X.3
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(width * WIDTH_SCALE, height * HEIGHT_SCALE, "Compute_Shader_Ray_Tracing", NULL, NULL);
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			getchar();
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW ( OpenGL Extension Wrangler Library )
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return -1;
		}

		// Ensure we can capture the keyboard being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Set background color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); // Accept fragment if it closer to the camera than the former one

		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable blending
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	}


	// Identify a Vertex Array Object
	GLuint VertexArray;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	// Load OBJ
#if 1
	obj["plane"] = Buffer("plane.obj");
	obj["cube"] = Buffer("cube.obj");
	obj["suzanne"] = Buffer("suzanne.obj");
	obj["teapot"] = Buffer("teapot.obj");
	obj["monster"] = Buffer("monster.obj");
#endif
	

	// --------------------
	// Load Texture Shader
	// --------------------
#if 1
	// Load Shader & Get a handle for uniform
	texture_shader.Load("MyTextureVertexShader.glsl", "MyTextureFragmentShader.glsl", 1);

	// Identify frame Buffer Object
	GLuint frameBuffer_texture[3];
	glGenFramebuffers(3, frameBuffer_texture);

	// Identify Texture
	GLuint renderedTexture[3];
	glGenTextures(3, renderedTexture);

	// Identify Depth Texture
	GLuint renderedTexture_depth[3];
	glGenRenderbuffers(3, renderedTexture_depth);

	for (int i = 0; i < 3; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_texture[i]);

		glBindTexture(GL_TEXTURE_2D, renderedTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);

		// filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Put Texture in the frame Buffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture[i], 0);

		glBindRenderbuffer(GL_RENDERBUFFER, renderedTexture_depth[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		// Put depth render Buffer in the frame Buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderedTexture_depth[i]);
	}

	// Set draw buffers.
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Failed to set Render To Texture.\n");
		getchar();
		glfwTerminate();
		return -1;
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
	glGenBuffers(1, &vertexBuffer_window);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_window);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_window_vertex_buffer_data), g_window_vertex_buffer_data, GL_STATIC_DRAW);
#endif


	// --------------------
	// Load Compute Shader
	// --------------------
#if 1
	// Load Shader & Get a handle for uniform
	compute_shader.Load("MyComputeShader.glsl");

	// Identify Texture
	GLuint computeTexture;
	glGenTextures(1, &computeTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, computeTexture);
	
	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glBindImageTexture(0, computeTexture, 0, GL_FLOAT, 0, GL_WRITE_ONLY, GL_RGBA16F);
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

			DrawSubWindow(0.0f, 0.0f, width, height, 1.0f, computeTexture);
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
		for (std::map<std::string, GLuint>::iterator it = texture.begin(); it != texture.end(); ++it)
			glDeleteTextures(1, &(it->second));

		// Texture Shader
		glDeleteFramebuffers(3, frameBuffer_texture);
		glDeleteTextures(3, renderedTexture);
		glDeleteRenderbuffers(3, renderedTexture_depth);
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

void DrawSubWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat m, GLuint t)
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

void TtoT(GLuint f, GLuint t, GLfloat m, GLuint b = 0)
{
	glBindFramebuffer(GL_FRAMEBUFFER, f);
	glViewport(0, 0, width, height);

	// Use shader
	texture_shader.Use();

	glUniform1f(texture_shader.GetVariable("Mode"), m);
	glUniform1i(texture_shader.GetVariable("BlurSize"), b);

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

void Draw_Scene_1()
{
	const int tri_num = 14;
	const std::vector<float> vertices = {
		-5.0f, -1.0f, -5.0f, -5.0f, -1.0f, 5.0f, 5.0f, -1.0f, -5.0f,
		5.0f, -1.0f, 5.0f, 5.0f, -1.0f, -5.0f, -5.0f, -1.0f, 5.0f,

		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};
	const std::vector<int> materials = {
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	GLuint vertexBuffer = 0, materialBuffer = 0;

	// Identify a vertex Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_DYNAMIC_COPY);

	// Identify a material Buffer Object
	glGenBuffers(1, &materialBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * materials.size(), &materials[0], GL_DYNAMIC_COPY);


	glUniform1i(compute_shader.GetVariable("tri_num"), vertices.size() / 9);

	// 1st buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuffer);

	// 2nd buffer : vertices
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialBuffer);
}