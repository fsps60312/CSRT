#pragma once
#include<glm/glm.hpp>
#include<common/control.hpp>
#include<common/matrix_d.hpp>
#include<functional>
#include<cmath>
class RigidBody {
private:
	void BackUp();
	void Restore();
public:
	glm::dvec3 position;
	glm::dvec3 velocity;
	glm::dvec3 force;
	double theta = 0;
	double omega = 0;
	double alpha = 0;

	glm::dvec3 _position;
	glm::dvec3 _velocity;
	glm::dvec3 _force;
	double _theta = 0;
	double _omega = 0;
	double _alpha = 0;

	double mass = 1;
	double momentOfInertia = 1.0 / 6 * 10; //rectangle's inertia: 1/12 m * (w*2 + h*2)


	glm::dvec3 GetVelocityAt(const glm::dvec3& relative_position)const;
	bool Advance(const double secs, std::function<bool(RigidBody*)>accepted = NULL);
};