#include<common/light.hpp>
std::vector<Light>Light::glob_lights(4, Light(glm::dvec3(0, 10, 5), 100));
glm::dvec3 Light::GetPosition()const { return position; }
double Light::GetPower()const { return power; }
Light::Light(const glm::dvec3& position, const double power) :position(position), power(power) {}