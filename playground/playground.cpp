#include <stdio.h>
#include <stdlib.h>
#include<chrono>
#include<iostream>


#include<common/camera.hpp>
#include <common/gl_check_error.hpp>
#include <common/buffer.hpp>
#include<common/shader.hpp>
#include<common/control.hpp>
#include<common/game.hpp>
#include<common/environment.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

int main(void)
{
	Camera camera;
	glm::vec3 light(0.0f, 10.0f, 5.0f);
	GLfloat  window_mode = 1.0f;
	Environment env;
	env.Init();

	auto model = Buffer(NOW_MODEL + std::string(".obj"));
	// For speed computation
	auto lastTime = std::chrono::steady_clock::now();
	int nbFrames = 0;

	do {

		// --------------------
		// Measure Speed
		// --------------------
		{
			auto currentTime = std::chrono::steady_clock::now();
			nbFrames++;
			if (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count() / 1000000000.0 >= 1.0) {
				// printf and reset
				printf("%f ms/frame\n", std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count() / 1000000.0 / double(nbFrames));
				nbFrames = 0;
				lastTime = std::chrono::steady_clock::now();
			}
		}


		// --------------------
		// Change Value
		// --------------------
		{
			// FoV
			if (env.IsKeyDown(GLFW_KEY_Z))
				camera.subFoV(1.0f);
			if (env.IsKeyDown(GLFW_KEY_X) == GLFW_PRESS)
				camera.addFoV(1.0f);
			camera.computeMatricesFromInputs(env);

			// Light Position
			if (env.IsKeyDown(GLFW_KEY_1))
				light.z += 0.01f;
			if (env.IsKeyDown(GLFW_KEY_2))
				light.x += 0.01f;
			if (env.IsKeyDown(GLFW_KEY_3))
				light.y += 0.01f;
			if (env.IsKeyDown(GLFW_KEY_4))
				light.y -= 0.01f;
			if (env.IsKeyDown(GLFW_KEY_5))
				light.x -= 0.01f;
			if (env.IsKeyDown(GLFW_KEY_6))
				light.z -= 0.01f;

			// Window Mode
			if (env.IsKeyDown(GLFW_KEY_F1))
				window_mode = 1.0f;
			if (env.IsKeyDown(GLFW_KEY_F2))
				window_mode = 2.0f;
			if (env.IsKeyDown(GLFW_KEY_F3))
				window_mode = 3.0f;
			if (env.IsKeyDown(GLFW_KEY_F4))
				window_mode = 4.0f;
			if (env.IsKeyDown(GLFW_KEY_F5))
				window_mode = 5.0f;
			if (env.IsKeyDown(GLFW_KEY_F6))
				window_mode = 6.0f;
			if (env.IsKeyDown(GLFW_KEY_F7))
				window_mode = 7.0f;
		}

		auto& compute_shader = env.GetComputeShader();
		// --------------------
		// Pass Value
		// --------------------
		{
			// Compute Shader
			compute_shader.Use();
			glUniform3f(compute_shader.GetVariable("eye"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
			glUniform3f(compute_shader.GetVariable("view"), camera.getDirection().x, camera.getDirection().y, camera.getDirection().z);
			glUniform3f(compute_shader.GetVariable("up"), camera.getUp().x, camera.getUp().y, camera.getUp().z);
			glUniform3f(compute_shader.GetVariable("light"), light.x, light.y, light.z);
			glUniform1f(compute_shader.GetVariable("fov"), camera.getFoV());
			glUniform1i(compute_shader.GetVariable("tri_num"), model.GetTriangleNum());
			model.Send();
		}
		env.DispatchShaders();


	} // Check if the ESC key was pressed or the window was closed
	while (!env.IsKeyDown(GLFW_KEY_ESCAPE) &&
		!env.IsWindowClosed());

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
