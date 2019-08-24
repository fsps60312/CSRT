#include<common/game.hpp>
void Game::Launch(){}
void Game::Pause(){}
void Game::Resume(){}
void Game::ReadSaveFile(std::ifstream& file_stream){}
void Game::WriteSaveFile(std::ofstream& file_stream){}
void Game::Render(){
	world.SendToShader();
}
void Game::Advance(std::chrono::nanoseconds timespan) {
}