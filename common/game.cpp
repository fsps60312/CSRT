#include<common/game.hpp>
#include<common/control.hpp>
void Game::Init() {
	model = BufferSystem(NOW_MODEL + std::string(".obj"));
}
void Game::Launch() {}
void Game::Pause(){}
void Game::Resume(){}
void Game::ReadSaveFile(std::ifstream& file_stream){}
void Game::WriteSaveFile(std::ofstream& file_stream){}
void Game::Render(Environment &env){
	camera.computeMatricesFromInputs(env);
	// Compute Shader
	auto compute_shader = env.GetComputeShader();
	compute_shader.Use();
	glUniform3f(compute_shader.GetVariable("eye"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	glUniform3f(compute_shader.GetVariable("view"), camera.getDirection().x, camera.getDirection().y, camera.getDirection().z);
	glUniform3f(compute_shader.GetVariable("up"), camera.getUp().x, camera.getUp().y, camera.getUp().z);
	glUniform3f(compute_shader.GetVariable("light"), light.x, light.y, light.z);
	glUniform1f(compute_shader.GetVariable("fov"), camera.getFoV());
	glUniform1i(compute_shader.GetVariable("tri_num"), model.GetTriangleNum());
	model.Send();
	world.SendToShader();
}
void Game::Advance(std::chrono::nanoseconds timespan) {
}
void Game::ReceiveKey(Keys key) {
	switch (key) {
	case Keys::X:
		camera.addFoV(1.0f); break;
	case Keys::Z:
		camera.subFoV(1.0f); break;
	case Keys::D1:
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
		light.z -= 1.0f; break;
	case Keys::F1:break;
	case Keys::F2:break;
	case Keys::F3:break;
	case Keys::F4:break;
	case Keys::F5:break;
	case Keys::F6:break;
	case Keys::F7:break;
	}
}