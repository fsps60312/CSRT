#pragma once
#include<common/visible_object.hpp>
#include<common/mylib.hpp>
#include<common/control.hpp>
#include<cmath>
namespace effects {
	class Fume :public VisibleObject {
	public:
		class FumesParent :public VisibleObject {
		public:
			std::vector<VisibleObject*>to_erase;
			void Update(const double secs)override;
		};
	public:
		static FumesParent* fumes_parent;
		static void AddFume(const glm::dvec3& initial_position, const glm::dvec3& speed, const double initial_radius, const double final_radius, const double initial_alpha, const double period);
	private:
		const glm::dvec3 speed;
		const double initial_radius, final_radius;
		const double total_period;
		const double omega, initial_alpha;
		double theta;
		int material_counter;
		glm::dvec3 position;
		double time_passed;
		std::vector<Triangle>GetTriangles();
		glm::dmat4 GetTransform()const;
		int GetMaterial();
	public:
		void Update(const double secs)override;
		Fume(const glm::dvec3& initial_position, const glm::dvec3& speed, const double initial_radius, const double final_radius, const double initial_alpha, const double period);
	};
}