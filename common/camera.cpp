// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "camera.hpp"

#include <stdio.h>



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

void Camera::computeMatricesFromInputs(GLFWwindow *window){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (MOVE_VIEW) {
		// Reset mouse position for next frame
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(WIDTH / 2 - xpos);
		verticalAngle += mouseSpeed * float(HEIGHT / 2 - ypos);
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
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}