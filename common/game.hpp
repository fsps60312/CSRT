#pragma once

#include<common/world.hpp>

#include<iostream>
#include<chrono>

class Game {
private:
	int money = 150;
	World world;
public:
	void Launch();
	void Pause();
	void Resume();
	void ReadSaveFile(std::ifstream &file_stream);
	void WriteSaveFile(std::ofstream &file_stream);
	void Render();
	void Advance(std::chrono::nanoseconds timespan);
};