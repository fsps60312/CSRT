#pragma once
#include<common/visible_object.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/matrix_d.hpp>
#include<common/control.hpp>
#include<common/rigid_body.hpp>
#include<common/block/blocks.hpp>
#include<cmath>
namespace pod {
	class PodTracks :public VisibleObject {
	public:
		class Track :public VisibleObject {
		public:
			class Tooth :public VisibleObject {
			private:
				glm::vec3 position;
				std::vector<Triangle>GetTriangles()const;
			public:
				Tooth(const glm::vec3& position);
				glm::vec3 GetPosition()const;
				void SetPosition(const glm::vec3& position);
			};
			class Gear :public VisibleObject {
			private:
				PodInterface* pod;
				Track* track;
				RigidBody rb;
				glm::dmat4 pre_matrix_y = matrix::IdentityD(), pre_matrix_z = matrix::IdentityD();
				const glm::dvec3 relative_position;
				double on_ground_countdown = 0;
				std::vector<Triangle>GetTriangles(const double radius)const;
				void RotateYAlongWithPod();
				void ApplyReactForceWithPod();
				void InverseVelocityIfCollideWithBlocks();
				bool IsRigidBodyMoveTooMuch(const double secs)const;
				void MoveGearBackToTouchSurface(const glm::dvec3& dir);
				void AdvanceRigidBody(const double secs);
				void Rotate(const double len);
				block::Block* collided;
			public:
				const double radius;
				const double suspension_hardness;
				const double mass;
				block::Block* GetCollided()const;
				bool IsOnGround()const;
				glm::dvec3 GetPosition()const;
				glm::dvec3 GetDesiredPosition()const;
				glm::dvec3 GetReactForce()const;
				void ApplyTranslate(const glm::dvec3& offset);
				void PrepareForRound()override;
				void Update(const double secs)override;
				void Advance(const double secs)override;
				Gear(const glm::dvec3& relative_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod, Track* track);
			};
		private:
			PodInterface* pod;
			std::vector<Gear*> gears, ground_gears;
			Gear* front_gear;
			std::vector<Tooth*>teeth;
			double track_cycle_speed = 1;
			double track_cycle_position = 0;
			void UpdateTrackSpeed(const double secs);
			glm::dvec3 GetGearToSubchainVector(Gear* a, Gear* b)const;
			double GetChainLength(const std::vector<Gear*>& chain)const;
			glm::vec3 GetToothPosition(const std::vector<Gear*>& chain, const double ratio);
		public:
			glm::dvec3 GetTotalReactForce()const;
			bool IsOnGround()const;
			bool IsAnyGearOnGround()const;
			double GetTrackSpeed()const;
			void AddTrackSpeed(const double dv);
			void Advance(const double secs)override;
			block::Block* CollideFront()const;
			void ApplyTranslate(const glm::dvec3& offset);
			Track(PodInterface* pod, const glm::dvec3& offset);
		};
	private:
		Track* left_track, * rigt_track;
	public:
		bool IsOnGround()const;
		glm::dvec3 GetTotalReactForce()const;
		block::Block* CollideFront()const;
		void ApplyTranslate(const glm::dvec3& offset);
		PodTracks(PodInterface* pod, const glm::dvec3& offset);
	};
}