#pragma once
#include<common/visible_object.hpp>
#include<common/pod/pod_interface.hpp>
#include<common/matrix_d.hpp>
namespace pod {
	class PodTracks :public VisibleObject {
	private:
	public:
		class Track :public VisibleObject {
		private:
		public:
			Track(PodInterface* pod, const glm::vec3& offset);
			class Tooth :public VisibleObject {
			private:
				glm::vec3 position;
			public:
				Tooth(const glm::vec3& position);
				glm::vec3 GetPosition()const;
				void SetPosition(const glm::vec3& position);
			};
		};
	};
}