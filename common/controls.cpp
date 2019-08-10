// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

#include <stdio.h>

#define VIEWMODE 0

#define WIDTH  512.0f
#define HEIGHT 512.0f

float getWidth() {
	return WIDTH;
}
float getHeight() {
	return HEIGHT;
}

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position( 0.0f, 0.0f, 2.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f; // +Z to +X (Math : +X to +Y)
// Initial vertical angle : none
float verticalAngle = 0.0f; // +X to + Y (Math : +Z to +Y)
// Initial Field of View
float initialFoV = 90.0f;

glm::vec3 getPosition() {
	return position;
}
float getFoV() {
	return initialFoV;
}
void addFoV(float num) {
	float tempFoV = initialFoV + num;
	initialFoV = tempFoV > 90.0f ? 90.0f : tempFoV;
}
void subFoV(float num) {
	float tempFoV = initialFoV - num;
	initialFoV = tempFoV < 30.0f ? 30.0f : tempFoV;
}

float speed = 5.0f; // 3 units / second
float mouseSpeed = 0.005f;
glm::vec3 direction;
glm::vec3 right;
glm::vec3 up;

glm::vec3 getDirection() {
	return direction;
}
glm::vec3 getUp() {
	return up;
}
void dPosition(float a, float b, float c)
{
	position += glm::vec3(a, b, c);
}

void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(WIDTH/2 - xpos);
	verticalAngle   += mouseSpeed * float(HEIGHT/2 - ypos);

	//horizontalAngle = 3.14f, verticalAngle = 0.0f;
	//horizontalAngle = 0.0f, verticalAngle = 0.0f;
	//horizontalAngle = 2.5f, verticalAngle = -0.5f;
	//printf("horizontalAngle = %.2f, verticalAngle = %.2f\n", horizontalAngle, verticalAngle);
	//printf("position = %.2f, %.2f, %.2f\n", position[0], position[1], position[2]);

#if VIEWMODE == 0
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
#elif VIEWMODE == 1
	direction = glm::normalize(glm::vec3(0.0f) - position);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::cross(direction, up);
	up = glm::cross(right, direction);
#endif

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

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f, 0.01f, 100.0f);
	// Camera matrix

#if VIEWMODE == 0
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
#elif VIEWMODE == 1
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		glm::vec3(0.0f), // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
#endif

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}