#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <common/gl_check_error.hpp>
#include<common/game.hpp>
#include<common/environment.hpp>

#include <stdio.h>
#include <stdlib.h>
#include<chrono>
#include<iostream>
#include <float.h>
unsigned int fp_control_state = _controlfp(_EM_INEXACT | _EM_UNDERFLOW, _MCW_EM);

void ReceiveKeys(Game &game);

int main(void)
{
	environment::Init();
	Game game;
	game.Init();

	// For speed computation
	auto lastDebugTime = std::chrono::steady_clock::now();
	auto lastTime = std::chrono::steady_clock::now();
	int nbFrames = 0;

	game.Launch();

	do {

		// --------------------
		// Measure Speed
		// --------------------
		auto currentTime = std::chrono::steady_clock::now();
		{
			nbFrames++;
			if (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastDebugTime).count() / 1000000000.0 >= 1.0) {
				// printf and reset
				printf("%f ms/frame\n", std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastDebugTime).count() / 1000000.0 / double(nbFrames));
				nbFrames = 0;
				lastDebugTime = currentTime;
			}
		}
		game.Advance(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime));
		lastTime = currentTime;
		ReceiveKeys(game);
		game.Render();
		environment::DispatchShaders();


	} // Check if the ESC key was pressed or the window was closed
	while (!environment::IsKeyDown(GLFW_KEY_ESCAPE) &&
		!environment::IsWindowClosed());

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void ReceiveKeys(Game& game) {
	// FoV
	if (environment::IsKeyDown(GLFW_KEY_Z))game.ReceiveKey(Game::Keys::Z);
	if (environment::IsKeyDown(GLFW_KEY_X))game.ReceiveKey(Game::Keys::X);


	// Light Position
	if (environment::IsKeyDown(GLFW_KEY_1))game.ReceiveKey(Game::Keys::D1);
	if (environment::IsKeyDown(GLFW_KEY_2))game.ReceiveKey(Game::Keys::D2);
	if (environment::IsKeyDown(GLFW_KEY_3))game.ReceiveKey(Game::Keys::D3);
	if (environment::IsKeyDown(GLFW_KEY_4))game.ReceiveKey(Game::Keys::D4);
	if (environment::IsKeyDown(GLFW_KEY_5))game.ReceiveKey(Game::Keys::D5);
	if (environment::IsKeyDown(GLFW_KEY_6))game.ReceiveKey(Game::Keys::D6);

	// Window Mode
	if (environment::IsKeyDown(GLFW_KEY_F1))game.ReceiveKey(Game::Keys::F1);
	if (environment::IsKeyDown(GLFW_KEY_F2))game.ReceiveKey(Game::Keys::F2);
	if (environment::IsKeyDown(GLFW_KEY_F3))game.ReceiveKey(Game::Keys::F3);
	if (environment::IsKeyDown(GLFW_KEY_F4))game.ReceiveKey(Game::Keys::F4);
	if (environment::IsKeyDown(GLFW_KEY_F5))game.ReceiveKey(Game::Keys::F5);
	if (environment::IsKeyDown(GLFW_KEY_F6))game.ReceiveKey(Game::Keys::F6);
	if (environment::IsKeyDown(GLFW_KEY_F7))game.ReceiveKey(Game::Keys::F7);
}