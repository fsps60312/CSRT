#include <common/camera.hpp>



int Camera::getWidth() {
	return WIDTH;
}
int Camera::getHeight() {
	return HEIGHT;
}



glm::vec3 Camera::getPosition() {
	return position;
}
float Camera::getFoV() {
	return initialFoV;
}
void Camera::addFoV(float num) {
	float tempFoV = initialFoV + num;
	initialFoV = tempFoV > 90.0f ? 90.0f : tempFoV;
}
void Camera::subFoV(float num) {
	float tempFoV = initialFoV - num;
	initialFoV = tempFoV < 30.0f ? 30.0f : tempFoV;
}


glm::vec3 Camera::getDirection() {
	return direction;
}
glm::vec3 Camera::getUp() {
	return up;
}
void Camera::dPosition(float a, float b, float c)
{
	position += glm::vec3(a, b, c);
}

void Camera::computeMatricesFromInputs(Environment &env){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	const glm::dvec2 mouse_pos = env.GetCursorPos();
	if (MOVE_VIEW) {
		// Reset mouse position for next frame
		env.SetCursorPos(glm::dvec2(WIDTH / 2, HEIGHT / 2));

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(WIDTH / 2 - mouse_pos.x);
		verticalAngle += mouseSpeed * float(HEIGHT / 2 - mouse_pos.y);
		verticalAngle = glm::clamp(verticalAngle, -glm::pi<float>() / 2.0f, glm::pi<float>() / 2.0f);
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	up = glm::cross( right, direction );

	// Move forward
	if (env.IsKeyDown(GLFW_KEY_UP ) ){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (env.IsKeyDown(GLFW_KEY_DOWN )){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (env.IsKeyDown(GLFW_KEY_RIGHT ) ){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (env.IsKeyDown(GLFW_KEY_LEFT ) ){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}