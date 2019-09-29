#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<common/rigid_body.hpp>
#include<common/control.hpp>
#include<vector>
namespace pod {
	class PodBody :VisibleObject {
	private:
		const static double body_radius;
		std::vector<glm::mat3>GetTriangles();
		double rotation_y = 0;
		double desired_rotation_y = 0;
		void MaintainRotationZ(const double secs);
		void MaintainRotationY(const double secs);
		void MaintainRigidBody(const double secs);
		RigidBody rb;
	public:
		PodBody();
		void Advance(const double secs)override;
	};
}