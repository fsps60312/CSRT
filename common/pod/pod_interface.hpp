#pragma once
#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<common/rigid_body.hpp>
class PodInterface:public VisibleObject {
public:
	virtual bool IsOnGround()const abstract;
	virtual bool IsPodStopped()const abstract;
	virtual glm::dmat4 GetMatrixY()const abstract;
	virtual glm::dmat4 GetMatrixZ()const abstract;
	virtual glm::dmat4 GetMatrixT()const abstract;
	virtual RigidBody* GetRigidBody() abstract;
};