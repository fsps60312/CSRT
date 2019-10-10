#pragma once
#include<glm/glm.hpp>
#include<random>
namespace mylib {
	class Rand {
	private:
		static std::mt19937 rand;
		static std::uniform_real_distribution<double> unif;
	public:
		static uint32_t NextUint();
		static double NextDouble();
	};
	void SmoothTo(double& v, const double t, const double secs, const double timeToHalf);
	void SmoothTo(glm::dvec3& v, const glm::dvec3& t, const double secs, const double timeToHalf);
	glm::dvec3 FromAngular(const double radius, const double theta, const double z = 0);
}