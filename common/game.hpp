#pragma once

#include<common/gpu.hpp>
#include<common/control.hpp>
#include<common/camera.hpp>
#include<common/mylib.hpp>
#include<iostream>
#include<chrono>

class Game {
private:
	int money = 150;
	GPU gpu;
public:
	enum class Keys {
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