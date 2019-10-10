#include<common/matrix_d.hpp>
namespace matrix {
	double AngleBetween(const glm::dvec3 &a, const glm::dvec3 &b) {
		return glm::acos(glm::clamp(glm::dot(glm::normalize(a), glm::normalize(b)), -1.0, 1.0));
	}
	glm::dmat4 Inverse(const glm::dmat4 &mat) {
		return glm::inverse(mat);
	}
	glm::dmat4 IdentityD() {
		return glm::dmat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}
	glm::dvec3 Multiply(const glm::dmat4& matrix, const glm::dvec3& v) {
		const glm::dvec4 res = matrix * glm::dvec4(v, 1.0f);
		return glm::dvec3(res.x, res.y, res.z);
	}
	glm::dmat4 ScaleD(const glm::dvec3& scale) {
		assert(!glm::any(glm::isnan(scale)));
		assert(!glm::any(glm::isinf(scale)));
		return glm::dmat4(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1
		);
	}
	glm::dmat4 TranslateD(const glm::dvec3& offset) {
		assert(!glm::any(glm::isnan(offset)));
		assert(!glm::any(glm::isinf(offset)));
		return glm::dmat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			offset.x, offset.y, offset.z, 1
		);
	}
	glm::dmat4 RotateD(const glm::dvec3& axis, const double theta) {
		//https://zh.wikipedia.org/zh-tw/旋转矩阵
		return glm::dmat4(
			std::cos(theta) + (1 - std::cos(theta)) * axis.x * axis.x,
			(1 - std::cos(theta)) * axis.y * axis.x + std::sin(theta) * axis.z,
			(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
			0,
			(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
			std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
			(1 - std::cos(theta)) * axis.z * axis.y + std::sin(theta) * axis.x,
			0,
			(1 - std::cos(theta)) * axis.x * axis.z + std::sin(theta) * axis.y,
			(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
			std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
			0,
			0, 0, 0, 1
		);
		return glm::dmat4(
			std::cos(theta) + (1 - std::cos(theta)) * axis.x * axis.x,
			(1 - std::cos(theta)) * axis.x * axis.y - std::sin(theta) * axis.z,
			(1 - std::cos(theta)) * axis.x * axis.z + std::sin(theta) * axis.y,
			0,
			(1 - std::cos(theta)) * axis.y * axis.x + std::sin(theta) * axis.z,
			std::cos(theta) + (1 - std::cos(theta)) * axis.y * axis.y,
			(1 - std::cos(theta)) * axis.y * axis.z - std::sin(theta) * axis.x,
			0,
			(1 - std::cos(theta)) * axis.z * axis.x - std::sin(theta) * axis.y,
			(1 - std::cos(theta)) * axis.z * axis.y + std::sin(theta) * axis.x,
			std::cos(theta) + (1 - std::cos(theta)) * axis.z * axis.z,
			0,
			0, 0, 0, 1
		);
	}
}