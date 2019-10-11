#pragma once
#include<glm/glm.hpp>
#include<vector>
class Light {
public:
	static std::vector<Light>glob_lights;
private:
	glm::dvec3 position;
	double power;
public:
	glm::dvec3 GetPosition()const;
	double GetPower()const;
	Light(const glm::dvec3& position, const double power);
};