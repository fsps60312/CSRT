#pragma once
#include<common/visible_object.hpp>
#include<common/control.hpp>
#include<common/pod/pod_interface.hpp>
#include<vector>
#include<cmath>
namespace pod {
	class PodDrill:public VisibleObject {
	public:
		class Blade:public VisibleObject {
		private:
			const double radius;
			const double rotate_angle, cone_angle, attack_angle;
			VisibleObject* blade, * support;
			std::vector<Triangle>GetBladeTriangles()const;
			std::vector<Triangle>GetSupportTriangles()const;
		public:
			void SetFoldState(const double fold_state);
			Blade(const double radius, const double rotate_angle, const double cone_angle, const double attack_angle);
		};
	private:
		PodInterface* pod;
		double fold_state = 0;
		const double radius, cone_angle;
		glm::dmat4 basic_transform;
		std::vector<Blade*>blades;
		void SetFoldState(const double fold_state);
	public:
		PodDrill(PodInterface* pod, const double radius, const int blade_count, const glm::dmat4& basic_transform);
		void Update(const double secs)override;
	};
}