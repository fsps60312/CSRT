#include<common/pod/pod_tracks.hpp>
namespace pod {
	void PodTracks::Track::Gear::Update() {
		// TODO
	}
	void PodTracks::Track::Gear::Advance(const double secs) {
		const glm::dmat4& mat_y = pod->GetMatrixY(), & mat_z = pod->GetMatrixZ(), & mat_t = pod->GetMatrixT();
		const glm::dmat4& mat_trans = matrix::Inverse(pre_matrix_y * mat_z * mat_t) * mat_y * mat_z * mat_t;
		const glm::dvec3& new_position = matrix::Multiply(mat_trans, rb.position);
		const glm::dvec3& new_velocity = matrix::Multiply(mat_trans, rb.velocity);
		//RB.position.X = newPosition.X; RB.position.Z = newPosition.Z;
		rb.position = new_position;
		rb.velocity = new_velocity;
		//RB.velocity.X = newVelocity.X; RB.velocity.Y = newVelocity.Y;
		pre_matrix_y = mat_y;
		transform = matrix::TranslateD(rb.position) * mat_y * matrix::RotateD(glm::dvec3(0, 0, -1), rb.theta);
	}
	glm::dvec3 PodTracks::Track::Gear::GetDesiredPosition()const {
		return  matrix::Multiply(pod->GetMatrixY() * pod->GetMatrixZ() * pod->GetMatrixT(), relative_position);
	}
	glm::dvec3 PodTracks::Track::Gear::GetPosition()const {
		return rb.position;
	}
	std::vector<Triangle>PodTracks::Track::Gear::GetTriangles(const double radius)const {
		const int n = 10;
		std::vector<glm::dvec3> positions;
		for (int i = 0; i < n; i++) positions.push_back(glm::dvec3(radius * std::cos(2 * PI * i / n), radius * std::sin(2 * PI * i / n), 0));
		std::vector<Triangle>ret;
		for (int i = 2; i < n; i++) {
			ret.push_back(Triangle(glm::mat3(positions[0], positions[i - 1LL], positions[i])));
			ret.push_back(Triangle(glm::mat3(positions[i], positions[i - 1LL], positions[0])));
		}
		return ret;
	}
	PodTracks::Track::Gear::Gear(const glm::dvec3& relative_position, const double radius, const double suspension_hardness, const double mass, PodInterface* pod) :
		relative_position(relative_position),
		radius(radius),
		suspension_hardness(suspension_hardness),
		mass(mass),
		pod(pod),
		pre_matrix_y(pod->GetMatrixY()),
		VisibleObject(GetTriangles(radius)) {
		rb = RigidBody();
		rb.mass = mass;
		rb.position = GetDesiredPosition();
		transform = matrix::TranslateD(relative_position);
	}
}