#pragma once
#include<common/control.hpp>
#include<common/environment.hpp>

namespace camera {
	int GetWidth();
	int GetHeight();

	void ComputeMatricesFromInputs();

	double GetFoV();
	void AddFoV(double num);
	void SubFoV(double num);
	glm::dvec3 GetPosition();
	glm::dvec3 GetDirection();
	glm::dvec3 GetUp();
	void SetPosition(const glm::dvec3& camera_position);
	void SetDirection(const glm::dvec3& camera_direction);
}