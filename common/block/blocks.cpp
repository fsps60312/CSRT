#include<common/block/blocks.hpp>
namespace block {
	Blocks Blocks::instance(glm::dvec3(0, 0, -constants::block_depth / 2));
	bool IsCollidable(const glm::dvec3& position) {
		return Blocks::instance.IsCollidable(glm::dvec2(position.x, position.y));
	}
	bool Blocks::IsCollidable(const glm::dvec2& position)const {
		const int x = (int)std::floor((position.x - anchor.x) / constants::block_width);
		const int y = (int)std::floor((position.y - anchor.y) / constants::block_height);
		return IsCollidable(x, y);
	}
	bool Blocks::IsCollidable(const int x, const int y) const{
		return y <= -5 || y >= 15 || x <= -15 || x >= 15 || (x >= -1 && x <= 1 && y == 2);
	}
	void Blocks::RecycleBlock(Block* b) {
		if (b == NULL)return;
		const size_t erased = children.erase(b);
		assert(erased == 1);
		delete b;
	}
	void Blocks::RemoveXMin() {
		for (const std::pair<int, Block*>& b : blocks.front().second)RecycleBlock(b.second);
		blocks.pop_front();
	}
	void Blocks::RemoveXMax() {
		for (const std::pair<int, Block*>& b : blocks.back().second)RecycleBlock(b.second);
		blocks.pop_back();
	}
	void Blocks::RemoveYMin() {
		for (std::pair<int, std::deque<std::pair<int, Block*>>>& bs : blocks) {
			RecycleBlock(bs.second.front().second);
			bs.second.pop_front();
		}
		if (blocks.front().second.empty())blocks.clear();
	}
	void Blocks::RemoveYMax() {
		for (std::pair<int, std::deque<std::pair<int, Block*>>>& bs : blocks) {
			RecycleBlock(bs.second.back().second);
			bs.second.pop_back();
		}
		if (blocks.front().second.empty())blocks.clear();
	}
	Block* Blocks::NewBlock(const int xi, const int yi) {
		if (!IsCollidable(xi, yi))return NULL;
		const glm::dvec3& position = anchor + glm::dvec3(constants::block_width * xi, constants::block_height * yi, 0.0);
		const glm::dvec3& size = glm::dvec3(constants::block_width - 0.2, constants::block_height - 0.2, constants::block_depth);
		Block* blk = new Block(position, size, Block::Type::General);
		children.insert(blk);
		return blk;
	}
	void Blocks::AppendYMin() {
		for (auto& bs : blocks) {
			const int yi = bs.second.front().first - 1;
			bs.second.push_front(std::make_pair(yi, NewBlock(bs.first, yi)));
		}
	}
	void Blocks::AppendYMax() {
		for (auto& bs : blocks) {
			const int yi = bs.second.back().first + 1;
			bs.second.push_back(std::make_pair(yi, NewBlock(bs.first, yi)));
		}
	}
	void Blocks::AppendXMin() {
		const int xi = blocks.front().first - 1;
		std::deque<std::pair<int, Block*>>new_list;
		for (auto& b : blocks.front().second)new_list.push_back(std::make_pair(b.first, NewBlock(xi, b.first)));
		blocks.push_front(make_pair(xi, new_list));
	}
	void Blocks::AppendXMax() {
		const int xi = blocks.back().first + 1;
		std::deque<std::pair<int, Block*>>new_list;
		for (auto& b : blocks.back().second)new_list.push_back(std::make_pair(b.first, NewBlock(xi, b.first)));
		blocks.push_back(make_pair(xi, new_list));
	}
	void Blocks::RegionOnXYPlane(double& x_min, double& x_max, double& y_min, double& y_max)const {
		const glm::dvec3& camera_dir = camera::GetDirection();
		const glm::dvec3& up = camera::GetUp();
		const glm::dvec3& dx = glm::normalize(glm::cross(camera_dir, up));
		const glm::dvec3& dy = glm::normalize(glm::cross(camera_dir, dx)) / double(SCREEN_WIDTH) * double(SCREEN_HEIGHT);
		const double fov = camera::GetFoV();
		const glm::dvec3& pos = camera::GetPosition();
		const glm::dvec3& center = pos + camera_dir / std::tan(fov / 2);
		x_min = y_min = DBL_MAX;
		x_max = y_max = -DBL_MAX;
		for (const glm::dvec3& target : { center - dx - dy,center - dx + dy,center + dx - dy,center + dx + dy }) {
			const glm::dvec3& dir = target - pos;
			const glm::dvec3& hit_point = pos - dir * (pos.z / dir.z);
			x_min = std::min(x_min, hit_point.x);
			y_min = std::min(y_min, hit_point.y);
			x_max = std::max(x_max, hit_point.x);
			y_max = std::max(y_max, hit_point.y);
		}
	}
	void Blocks::Update(const double secs) {
		double x_min, x_max, y_min, y_max;
		RegionOnXYPlane(x_min, x_max, y_min, y_max);
		const int
			x_mn = (int)std::floor((x_min - anchor.x) / constants::block_width) - 1,
			x_mx = (int)std::ceil((x_max - anchor.x) / constants::block_width) + 1,
			y_mn = (int)std::floor((y_min - anchor.y) / constants::block_height) - 1,
			y_mx = (int)std::ceil((y_max - anchor.y) / constants::block_height) + 1;
		while (!blocks.empty() && blocks.front().first < x_mn)RemoveXMin();
		while (!blocks.empty() && blocks.back().first > x_mx)RemoveXMax();
		while (!blocks.empty() && blocks.front().second.front().first < y_mn)RemoveYMin();
		while (!blocks.empty() && blocks.front().second.back().first > y_mx)RemoveYMax();
		if (blocks.empty())blocks.push_back(std::make_pair(x_mn, std::deque<std::pair<int, Block*>>{std::make_pair(y_mn, NewBlock(x_mn, y_mn))}));
		while (!blocks.empty() && blocks.front().second.front().first > y_mn)AppendYMin();
		while (!blocks.empty() && blocks.front().second.back().first < y_mx)AppendYMax();
		while (!blocks.empty() && blocks.front().first > x_mn)AppendXMin();
		while (!blocks.empty() && blocks.back().first < x_mx)AppendXMax();
	}
	Blocks::Blocks(const glm::dvec3& anchor) :anchor(anchor) {
	}
}