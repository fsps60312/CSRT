#pragma once
#include<common/visible_object.hpp>
#include<common/triangle.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/mylib.hpp>
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
				std::vector<Triangle>GetTriangles(const Types type, const double radius, const double theta, const double reversed);
			public:
				const std::map<Types, Description>descriptions = {
					{Types::BasicTriangle,Description({mylib::FromAngular(0,0),mylib::FromAngular(1,0),mylib::FromAngular(0.9,-7.0 / 180 * PI),mylib::FromAngular(0.2,-40 / 180 * PI)})}
				};
				void SetFoldAngle(const double theta);
				Blade(const Types type, const double radius, const double theta, const double reversed);
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
		public:
			bool reversed = false;
			double speed_ratio = 1;
			double theta_offset = 0;
			double height = 0;
			void SetFoldState(const double fold_state);
			void SetHeight(const double height);
			enum Types { Basic };
			const std::map<Types, Description>descriptions = {
				{Types::Basic,Description(Blade::Types::BasicTriangle,3)}
			};
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
		double GetAirFriction(const double air_speed);
		double moment_of_inertia = 1;
		double max_power = 50;
		double max_torque = 20;
		double folding_time = 0.5;
		double fold_state = 0;
		double height, max_height;
		std::vector<BladeSet*>bladesets;
		PodInterface* pod;
		double GetDownwardSpeed();
	public:
		enum Types { Basic };
		const std::map<Types, Description>descriptions = {{
				Types::Basic,Description({
						BladeSetDescription(BladeSet::Types::Basic,2.0,false,1),
						BladeSetDescription(BladeSet::Types::Basic,2.0,true,0.5)
					}
				)
			}
		};
		double GetLiftForce();
		PodPropeller(PodInterface* pod, const Types propeller_type);
	};
}