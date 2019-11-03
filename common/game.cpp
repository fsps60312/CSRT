#include<common/game.hpp>

void Game::Init() {
	gpu = GPU(/*NOW_MODEL + std::string(".obj")*/);
}
void Game::Launch() {}
void Game::Pause(){}
void Game::Resume(){}
void Game::ReadSaveFile(std::ifstream& file_stream){}
void Game::WriteSaveFile(std::ofstream& file_stream){}
void Game::Render(){
	camera::ComputeMatricesFromInputs();
	gpu.Send();
	gpu.Dispatch();
}
void Game::Advance(std::chrono::nanoseconds timespan) {
	if (timespan.count() == 0)return;
	const double secs = timespan.count() / 1000000000.0;
	if (secs > 1.0 / 60) {
		Advance(timespan / 2);
		Advance(timespan / 2);
	} else {
		gpu.obj->PrepareForRound();
		gpu.obj->Update(secs);
		gpu.obj->Advance(secs);
	}
	//model.obj->Advance(0.01);
}
void Game::ReceiveKey(Keys key) {
	switch (key) {
	/*case Keys::D1:
		light.x += 1.0f; break;
	case Keys::D2:	
		light.x -= 1.0f; break;
	case Keys::D3:	
		light.y += 1.0f; break;
	case Keys::D4:	
		light.y -= 1.0f; break;
	case Keys::D5:	
		light.z += 1.0f; break;
	case Keys::D6:	
		light.z -= 1.0f; break;*/
	case Keys::F1:break;
	case Keys::F2:break;
	case Keys::F3:break;
	case Keys::F4:break;
	case Keys::F5:break;
	case Keys::F6:break;
	case Keys::F7:break;
	}
}