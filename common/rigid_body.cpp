#include<common/rigid_body.hpp>
void RigidBody::Reset() {
	force = glm::dvec3(0);
	alpha = 0;
}
void RigidBody::Advance(const double secs) {
	BackUp();
	{
		const auto preV = velocity;
		const auto f = force;
		velocity += f / mass * secs;
		position += (preV + velocity) / 2.0 * secs;
	}
	{
		const auto preV = omega;
		const auto f = alpha;
		omega += f / momentOfInertia * secs;
		theta += (preV + omega) / 2.0 * secs;
		theta = fmod(theta, 2.0 * PI);
	}
}

glm::dvec3 RigidBody::GetVelocityAt(const glm::dvec3& relative_position)const {
	auto mat = matrix::RotateD(glm::dvec3(0, 0, 1), theta);
	return velocity + glm::cross(matrix::Multiply(mat, relative_position), glm::dvec3(0, 0, -1)) * omega;
}

void RigidBody::BackUp() {
	_position = position;
	_velocity = velocity;
	_force = force;
	_theta = theta;
	_omega = omega;
	_alpha = alpha;
}
void RigidBody::Restore() {
	position = _position;
	velocity = _velocity;
	force = _force;
	theta = _theta;
	omega = _omega;
	alpha = _alpha;
}