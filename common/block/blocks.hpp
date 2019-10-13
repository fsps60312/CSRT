#pragma once
#include<common/visible_object.hpp>
#include<common/block/block.hpp>
#include<common/control.hpp>
#include<common/camera.hpp>
#include<deque>
#include<map>
namespace block {
	bool IsCollidable(const glm::dvec3& position);
	Block* GetBlock(const glm::dvec3& position);
	bool Destroy(const Block* block);
	class Blocks :public VisibleObject {
	private:
		const glm::dvec3 anchor;
		std::deque<std::pair<int, std::deque<std::pair<int, Block*>>>>blocks;
		std::map<const Block*, std::pair<int, int>>block_positions;
		void RecycleBlock(Block* b);
		void RemoveXMin();
		void RemoveXMax();
		void RemoveYMin();
		void RemoveYMax();
		Block* NewBlock(const int xi, const int yi);
		void AppendXMin();
		void AppendXMax();
		void AppendYMin();
		void AppendYMax();
		bool Destroy(const int x, const int y);
	public:
		static Blocks* instance;
		static void RegionOnXYPlane(double& x_min, double& x_max, double& y_min, double& y_max);
		bool IsCollidable(const int x, const int y)const;
		bool IsCollidable(const glm::dvec2& position)const;
		Block* GetBlock(const glm::dvec2& position)const;
		Block* GetBlock(const int x, const int y)const;
		bool Destroy(const Block* block);
		Blocks(const glm::dvec3 &anchor);
		void Update(const double secs)override;
	};
}