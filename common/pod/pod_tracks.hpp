#pragma once
#include<common/visible_object.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/matrix_d.hpp>
#include<common/control.hpp>
#include<common/rigid_body.hpp>
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
				RigidBody rb;
				glm::dmat4 pre_matrix_y;
				const glm::dvec3 relative_position;
				std::vector<Triangle>GetTriangles(const double radius)const;
			public:
				const double radius;
				const double suspension_hardness;
				const double mass;
				Gear(const glm::dvec3& relative_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod);
				void Advance(const double secs)override;
				void Update()override;
				glm::dvec3 GetPosition()const;
				glm::dvec3 GetDesiredPosition()const;
			};
		private:
			PodInterface* pod;
			std::vector<Gear*> gears, ground_gears;
			std::vector<Tooth*>teeth;
			double track_speed = 1;
			double track_cycle_position = 0;
			glm::dvec3 GetChainTouchPoint(Gear* a, Gear* b)const;
			double GetChainLength(const std::vector<Gear*>& chain)const;
			glm::vec3 GetToothPosition(const std::vector<Gear*>& chain, const double ratio);
		public:
			Track(PodInterface* pod, const glm::dvec3& offset);
			void Advance(const double secs)override;
		};
	private:
		Track* left_track, * rigt_track;
	public:
		PodTracks(PodInterface* pod, const glm::dvec3& offset);
	};
}