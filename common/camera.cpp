#include <common/camera.hpp>

namespace camera {
	// Initial Field of View
	double initialFoV = 60.0 / 180.0 * PI;

	double speed = 5.0f; // 3 units / second
	double mouseSpeed = 0.005f;
	// Initial position : on +Z
	glm::dvec3 position = glm::dvec3(0.0, 0.0, 2.0);
	// Initial horizontal angle : toward -Z
	double horizontal_angle = PI; // +Z to +X (Math : +X to +Y)
	// Initial vertical angle : none
	double vertical_angle = 0; // +X to + Y (Math : +Z to +Y)
	glm::dvec3 direction = glm::dvec3(0, 0, -1);
	glm::dvec3 up = glm::dvec3(0, 1, 0);
	void SetPosition(const glm::dvec3& camera_position) {
		position = camera_position;
	}
	void SetDirection(const glm::dvec3& camera_direction) {
		direction = camera_direction;
	}
	glm::dvec3 GetPosition() {
		return position;
	}
	glm::dvec3 GetDirection() {
		return direction;
	}
	glm::dvec3 GetUp() {
		return up;
	}
	double GetFoV() {
		return initialFoV;
	}
	void AddFoV(double num) {
		double tempFoV = initialFoV + num;
		//initialFoV = tempFoV > 90.0 ? 90.0 : tempFoV;
	}
	void SubFoV(double num) {
		double tempFoV = initialFoV - num;
		//initialFoV = tempFoV < 30.0 ? 30.0 : tempFoV;
	}


	void ComputeMatricesFromInputs() {

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		double deltaTime = double(currentTime - lastTime);

		// Get mouse position
		const glm::dvec2 mouse_pos = environment::GetCursorPos();
		if (MOVE_VIEW) {
			// Reset mouse position for next frame
			environment::SetCursorPos(glm::dvec2(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0));

			// Compute new orientation
			horizontal_angle += mouseSpeed * double(SCREEN_WIDTH / 2.0 - mouse_pos.x);
			vertical_angle += mouseSpeed * double(SCREEN_HEIGHT / 2.0 - mouse_pos.y);
			vertical_angle = glm::clamp(vertical_angle, -PI / 2.0, PI / 2.0);
		}

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::dvec3(
			cos(vertical_angle) * sin(horizontal_angle),
			sin(vertical_angle),
			cos(vertical_angle) * cos(horizontal_angle)
		);

		// Right vector
		const glm::dvec3& right = glm::dvec3(
			sin(horizontal_angle - PI / 2.0),
			0,
			cos(horizontal_angle - PI / 2.0)
		);

		// Up vector
		up = glm::cross(right, direction);

		// Move forward
		if (environment::IsKeyDown(GLFW_KEY_T)) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (environment::IsKeyDown(GLFW_KEY_G)) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (environment::IsKeyDown(GLFW_KEY_H)) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (environment::IsKeyDown(GLFW_KEY_F)) {
			position -= right * deltaTime * speed;
		}
		// Move downward
		if (environment::IsKeyDown(GLFW_KEY_Y)) {
			position += up * deltaTime * speed;
		}
		// Move upward
		if (environment::IsKeyDown(GLFW_KEY_R)) {
			position -= up * deltaTime * speed;
		}

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.


		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}
}