#pragma once
#include<glm/glm.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/rigid_body.hpp>
#include<common/control.hpp>
#include<common/environment.hpp>
#include<common/mylib.hpp>
#include<iostream>
#include<vector>
namespace pod {
	class PodBody :public VisibleObject {
	private:
		const static double body_radius;
		std::vector<Triangle>GetTriangles();
		double rotation_y = 0;
		double desired_rotation_y = 0;
		void MaintainRotationZ(const double secs);
		void MaintainRotationY(const double secs);
		void MaintainRigidBody(const double secs);
		RigidBody rb;
		PodInterface* parent;
	public:
		PodBody(PodInterface *parent);
		void Advance(const double secs)override;
	};
}