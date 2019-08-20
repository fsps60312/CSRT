#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include<common/control.hpp>

class Camera {
private:

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f; // +Z to +X (Math : +X to +Y)
	// Initial vertical angle : none
	float verticalAngle = 0.0f; // +X to + Y (Math : +Z to +Y)
	// Initial Field of View
	float initialFoV = 90.0f;

	float speed = 5.0f; // 3 units / second
	float mouseSpeed = 0.005f;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
public:
	int getWidth();
	int getHeight();

	void computeMatricesFromInputs(GLFWwindow* window);

	glm::vec3 getPosition();
	float getFoV();
	void addFoV(float num);
	void subFoV(float num);

	glm::vec3 getDirection();
	glm::vec3 getUp();
	void dPosition(float a, float b, float c);
};

#endif