#include<common/visible_object.hpp>
namespace pod {
	class Pod :public VisibleObject {
	public:
		Pod();
		void Advance(const double secs)override;
	};
}