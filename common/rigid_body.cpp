#include<common/rigid_body.hpp>
void RigidBody::Update() {
	Restore();
}
bool RigidBody::Advance(const double secs, std::function<bool(RigidBody*)>is_accepted) {
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
	if (is_accepted == NULL || is_accepted(this)) {
		std::swap(_position, position);
		std::swap(_velocity, velocity);
		std::swap(_force, force);
		std::swap(_theta, theta);
		std::swap(_omega, omega);
		std::swap(_alpha, alpha);
		return true;
	} else {
		Restore();
		return false;
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