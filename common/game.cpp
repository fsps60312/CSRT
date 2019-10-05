#include<common/game.hpp>

void Game::Init() {
	model = BufferSystem(NOW_MODEL + std::string(".obj"));
}
void Game::Launch() {}
void Game::Pause(){}
void Game::Resume(){}
void Game::ReadSaveFile(std::ifstream& file_stream){}
void Game::WriteSaveFile(std::ofstream& file_stream){}
void Game::Render(){
	camera::ComputeMatricesFromInputs();
	// Compute Shader
	auto compute_shader = environment::GetComputeShader();
	compute_shader.Use();
	const glm::dvec3& camera_position = camera::GetPosition();
	const glm::dvec3& camera_direction = camera::GetDirection();
	const glm::dvec3& camera_up = camera::GetUp();
	glUniform3f(compute_shader.GetVariable("eye"), camera_position.x, camera_position.y, camera_position.z);
	glUniform3f(compute_shader.GetVariable("view"), camera_direction.x, camera_direction.y, camera_direction.z);
	glUniform3f(compute_shader.GetVariable("up"), camera_up.x, camera_up.y, camera_up.z);
	glUniform3f(compute_shader.GetVariable("light"), light.x, light.y, light.z);
	glUniform1f(compute_shader.GetVariable("fov"), camera::GetFoV());
	glUniform1i(compute_shader.GetVariable("tri_num"), model.GetTriangleNum());
	static std::default_random_engine rand(7122);
	//std::clog << rand() << std::endl;
	glUniform1ui(compute_shader.GetVariable("initial_random_seed"), rand());
	model.Send();
	world.SendToShader();
}
void Game::Advance(std::chrono::nanoseconds timespan) {
	const double secs = timespan.count() / 1000000000.0;
	model.obj->Update(secs);
	model.obj->Advance(secs);
	//model.obj->Advance(0.01);
}
void Game::ReceiveKey(Keys key) {
	switch (key) {
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