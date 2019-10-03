#pragma once
#include<common/visible_object.hpp>
#include<common/triangle.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/mylib.hpp>
#include<common/environment.hpp>
#include<cmath>
#include<map>
#include<vector>
namespace pod {
	class PodPropeller:public VisibleObject{
	public:
		class BladeSet :public VisibleObject {
		public:
			class Blade :public VisibleObject {
			public:
				class Description {
				public:
					std::vector<glm::dvec3>vertices;
					Description(const std::vector<glm::dvec3>& vertices);
				};
				enum Types { BasicTriangle };
			private:
				double fold_angle = 0;
				const double turn_down_angle;
				const double cycle_angle;
				glm::dmat4 GetTransform()const;
				std::vector<Triangle>GetTriangles(const Types type, const double radius, const double reversed);
			public:
				const static std::map<Types, Description>descriptions;
				void SetFoldAngle(const double theta);
				Blade(const Types type, const double radius, const double cycle_angle, const double turn_down_angle, const double reversed);
			};
			class Description {
			public:
				Blade::Types blade_type;
				int count;
				Description(const Blade::Types blade_type, const int count);
			};
		private:
			std::vector<Blade*>blades;
			PodPropeller* propeller;
			double theta = 0;
		public:
			bool reversed = false;
			double speed_ratio = 1;
			double theta_offset = 0;
			double height = 0;
			void SetFoldState(const double fold_state);
			void SetHeight(const double height);
			enum Types { Basic };
			const static std::map<Types, Description>descriptions;
			void Advance(const double secs)override;
			BladeSet(PodPropeller* propeller, const Types type, const double radius, const bool reversed, const double speed_ratio, const double theta_offset);
		};
		class BladeSetDescription {
		public:
			const BladeSet::Types type;
			const double radius;
			const bool reversed;
			const double height;
			const double speed_ratio;
			const double theta_offset;
			BladeSetDescription(const BladeSet::Types type, const double radius, const bool reversed, const double height, const double speed_ratio = 1, const double theta_offset = 0);
		};
		class Description {
		public:
			const std::vector<BladeSetDescription>bladesets;
			Description(const std::vector<BladeSetDescription>& bladesets);
		};
	private:
		double omega = 0;
		double friction = 1;
		double moment_of_inertia = 1;
		double max_power = 50;
		double max_torque = 20;
		double folding_time = 0.5;
		double fold_state = 0;
		double height, max_height;
		bool want_to_fold = false;
		std::vector<BladeSet*>bladesets;
		PodInterface* pod;
		const glm::dmat4 basic_transform;
		double GetAirFriction(const double air_speed);
		double GetDownwardSpeed();
	public:
		enum Types { Basic };
		const static std::map<Types, Description>descriptions;
		double GetOmega();
		double GetLiftForce();
		void SetFoldState(const double fold_state);
		void Advance(const double secs)override;
		PodPropeller(PodInterface* pod, const Types propeller_type, const glm::dmat4& basic_transform);
	};
}