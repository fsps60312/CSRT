#include<glm/glm.hpp>
namespace matrix {
	glm::dmat4 IdentityD();
	glm::dmat4 Inverse(const glm::dmat4& mat);
	glm::dmat4 RotateD(const glm::dvec3& axis, const double theta);
	glm::dmat4 TranslateD(const glm::dvec3& offset);
	glm::dvec3 Multiply(const glm::dmat4& matrix, const glm::dvec3& v);
	double AngleBetween(const glm::dvec3 &a, const glm::dvec3 &b);
}