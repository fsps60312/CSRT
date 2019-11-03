#pragma once
#include<common/visible_object.hpp>
#include<common/rigid_body.hpp>
#include<common/mylib.hpp>
#include<common/block/blocks.hpp>
namespace effects {
	class Gravel:public VisibleObject {
	public:
		class GravelsParent :public VisibleObject {
		public:
			std::vector<VisibleObject*>to_erase;
			void Advance(const double secs)override;
		};
	public:
		static GravelsParent* gravels_parent;
		static void AddGravel(const glm::dvec3& initial_position, const glm::dvec3& initial_speed);
	private:
		RigidBody rb;
		const double omega_x, omega_y, omega_z;
		double theta_x, theta_y, theta_z;
		std::vector<Triangle>GetTriangles()const;
		glm::dmat4 GetTransform()const;
	public:
		void Advance(const double secs)override;
		Gravel(const glm::dvec3& initial_position, const glm::dvec3& initial_speed);
	};
}