#pragma once
#include<common/visible_object.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/matrix_d.hpp>
#include<common/control.hpp>
#include<cmath>
namespace pod {
	class PodTracks :public VisibleObject {
	private:
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
			public:
				const glm::dvec3 desired_position;
				const double radius;
				const double suspension_hardness;
				const double mass;
				Gear(const glm::dvec3& desired_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod);
			};
		private:
			PodInterface* pod;
			std::vector<Gear*> gears, groundGears;
			glm::dvec3 GetChainTouchPoint(Gear* a, Gear* b)const;
			double GetChainLength(const std::vector<Gear*>& chain)const;
			glm::vec3 GetToothPosition(const std::vector<Gear*>& chain, const double ratio);
		public:
			Track(PodInterface* pod, const glm::vec3& offset);
		};
	};
}