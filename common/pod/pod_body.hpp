#pragma once
#include<glm/glm.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/rigid_body.hpp>
#include<common/control.hpp>
#include<common/environment.hpp>
#include<common/mylib.hpp>
#include<common/camera.hpp>
#include<common/pod/pod_propeller.hpp>
#include<iostream>
#include<vector>
#include<cmath>
namespace pod {
	class PodBody :public VisibleObject {
	private:
		const static double body_radius;
		std::vector<Triangle>GetTriangles()const;
		double rotation_y = 0;
		double desired_rotation_y = 0;
		void UpdateRotationZ();
		void UpdateRotationY();
		void UpdateRigidBody();
		void AdvanceCamera(const double secs);
		void AdvanceRigidBody(const double secs);
		RigidBody rb;
		PodInterface* pod;
		PodPropeller* propeller;
	public:
		glm::dmat4 GetMatrixY()const;
		glm::dmat4 GetMatrixZ()const;
		glm::dmat4 GetMatrixT()const;
		RigidBody* GetRigidBody();
		PodBody(PodInterface *parent);
		void Update()override;
		void Advance(const double secs)override;
	};
}