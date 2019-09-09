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

void ReceiveKeys(const Environment &env,Game &game);

int main(void)
{
	Environment env;
	env.Init();
	Game game;
	game.Init();

	// For speed computation
	auto lastTime = std::chrono::steady_clock::now();
	int nbFrames = 0;

	game.Launch();

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

		ReceiveKeys(env, game);
		game.Render(env);
		env.DispatchShaders();


	} // Check if the ESC key was pressed or the window was closed
	while (!env.IsKeyDown(GLFW_KEY_ESCAPE) &&
		!env.IsWindowClosed());

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void ReceiveKeys(const Environment& env, Game& game) {
	// FoV
	if (env.IsKeyDown(GLFW_KEY_Z))game.ReceiveKey(Game::Keys::Z);
	if (env.IsKeyDown(GLFW_KEY_X))game.ReceiveKey(Game::Keys::X);


	// Light Position
	if (env.IsKeyDown(GLFW_KEY_1))game.ReceiveKey(Game::Keys::D1);
	if (env.IsKeyDown(GLFW_KEY_2))game.ReceiveKey(Game::Keys::D2);
	if (env.IsKeyDown(GLFW_KEY_3))game.ReceiveKey(Game::Keys::D3);
	if (env.IsKeyDown(GLFW_KEY_4))game.ReceiveKey(Game::Keys::D4);
	if (env.IsKeyDown(GLFW_KEY_5))game.ReceiveKey(Game::Keys::D5);
	if (env.IsKeyDown(GLFW_KEY_6))game.ReceiveKey(Game::Keys::D6);

	// Window Mode
	if (env.IsKeyDown(GLFW_KEY_F1))game.ReceiveKey(Game::Keys::F1);
	if (env.IsKeyDown(GLFW_KEY_F2))game.ReceiveKey(Game::Keys::F2);
	if (env.IsKeyDown(GLFW_KEY_F3))game.ReceiveKey(Game::Keys::F3);
	if (env.IsKeyDown(GLFW_KEY_F4))game.ReceiveKey(Game::Keys::F4);
	if (env.IsKeyDown(GLFW_KEY_F5))game.ReceiveKey(Game::Keys::F5);
	if (env.IsKeyDown(GLFW_KEY_F6))game.ReceiveKey(Game::Keys::F6);
	if (env.IsKeyDown(GLFW_KEY_F7))game.ReceiveKey(Game::Keys::F7);
}