#ifndef CONTROLS_HPP
#define CONTROLS_HPP

float getWidth();
float getHeight();

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::vec3 getPosition();
float getFoV();
void addFoV(float num);
void subFoV(float num);

glm::vec3 getDirection();
glm::vec3 getUp();
void dPosition(float a, float b, float c);

#endif