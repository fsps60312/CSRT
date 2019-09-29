#include<glm/glm.hpp>
namespace mylib {
	void SmoothTo(double& v, const double t, const double secs, const double timeToHalf);
	void SmoothTo(glm::dvec3& v, const glm::dvec3& t, const double secs, const double timeToHalf);
}