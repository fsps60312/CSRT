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
		std::vector<Triangle>GetTriangles()const;
		double rotation_y = 0;
		double desired_rotation_y = 0;
		void DesiredRotationZ();
		void DesiredRotationY();
		void AdvanceRigidBody(const double secs);
		RigidBody rb;
		PodInterface* parent;
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