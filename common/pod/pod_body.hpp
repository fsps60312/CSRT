#pragma once
#include<glm/glm.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/rigid_body.hpp>
#include<common/control.hpp>
#include<common/environment.hpp>
#include<common/mylib.hpp>
#include<common/camera.hpp>
#include<common/pod/pod_propeller.hpp>
#include<common/pod/pod_drill.hpp>
#include<common/block/block.hpp>
#include<common/effects/fume.hpp>
#include<common/effects/gravel.hpp>
#include<common/light.hpp>
#include<iostream>
#include<vector>
#include<cmath>
namespace pod {
	class PodBody :public VisibleObject {
	private:
		enum class DigIntention { None, Left, Right, Down };
		block::Block* block_digging = NULL;
		const static double body_radius;
		double rotation_y = 0;
		double desired_rotation_y = 0;
		double on_ground_counter = 0.0;
		RigidBody rb;
		PodInterface* pod;
		PodPropeller* propeller;
		PodDrill* drill;
		std::vector<Triangle>GetTriangles()const;
		std::vector<Triangle>GetSpotLightTrianlges()const;
		DigIntention GetDigIntention()const;
		void UpdateDigIntention(const double secs);
		void UpdateRotationZ();
		void UpdateRotationY();
		void UpdateRigidBody();
		void UpdateFume(const double secs);
		void InverseVelocityIfCollideWithBlocks();
		void AdvanceCamera(const double secs);
		void AdvanceRigidBody(const double secs);
		bool IsRigidBodyMoveTooMuch(const double secs)const;
	public:
		glm::dmat4 GetMatrixY()const;
		glm::dmat4 GetMatrixZ()const;
		glm::dmat4 GetMatrixT()const;
		RigidBody* GetRigidBody();
		glm::dvec3 GetForce()const;
		double GetCorrectiveAlpha()const;
		void ApplyTranslate(const glm::dvec3& offset);
		PodBody(PodInterface *parent);
		void PrepareForRound()override;
		void Update(const double secs)override;
		void Advance(const double secs)override;
	};
}