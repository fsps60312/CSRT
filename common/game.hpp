#pragma once

#include<common/world.hpp>
#include<common/buffer_system.hpp>
#include<common/camera.hpp>

#include<iostream>
#include<chrono>

class Game {
private:
	int money = 150;
	World world;
	BufferSystem model;
	Camera camera;
	glm::vec3 light = glm::vec3(0.0f, 10.0f, 5.0f);
public:
	enum Keys {
		Z, X,
		D1, D2, D3, D4, D5, D6,
		F1, F2, F3, F4, F5, F6, F7
	};
	void Init();
	void Launch();
	void Pause();
	void Resume();
	void ReadSaveFile(std::ifstream &file_stream);
	void WriteSaveFile(std::ofstream &file_stream);
	void Render();
	void Advance(std::chrono::nanoseconds timespan);
	void ReceiveKey(Keys key);
};