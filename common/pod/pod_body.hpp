#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<vector>
namespace pod {
	class PodBody :VisibleObject {
	private:
		std::vector<glm::mat3>GetTriangles();
	public:
		PodBody();
	};
}