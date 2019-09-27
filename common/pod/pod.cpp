#include<common/pod/pod.hpp>
#include<common/pod/pod_body.hpp>
namespace pod {
	Pod::Pod() :VisibleObject() {
		this->children.push_back((VisibleObject*)new PodBody());
	}
	void Pod::Advance(const double secs) {
	}
}