#include<common/mylib.hpp>
#include<cmath>
namespace mylib {
	void SmoothTo(double& v, const double t, const double secs, const double timeToHalf)
	{
		///f = a x      ### f: location-velocity
		///F = S(1/f) = S(1/a x^-1) = ln(x)/a: time
		///secs/timeToHalf = (F(1)-F(x)) / (F(1)-F(0.5)) = ln(x) / ln(0.5)
		///ln(x) = secs / timeToHalf * ln(0.5)
		///x = Exp(secs / timeToHalf * ln(0.5))
		double x = std::exp(secs / timeToHalf * std::log(0.5));
		v = v * x + t * (1 - x);
	}
	void SmoothTo(glm::dvec3& v, const glm::dvec3& t, const double secs, const double timeToHalf) {
		SmoothTo(v.x, t.x, secs, timeToHalf);
		SmoothTo(v.y, t.y, secs, timeToHalf);
		SmoothTo(v.z, t.z, secs, timeToHalf);
	}
}