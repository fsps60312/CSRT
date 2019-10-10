#pragma once
#include<glm/glm.hpp>
#include<common/visible_object.hpp>
#include<common/rigid_body.hpp>
#include<common/block/blocks.hpp>
class PodInterface:public VisibleObject {
public:
	virtual bool IsOnGround()const abstract;
	virtual bool IsPodStableOnGround()const abstract;
	virtual glm::dmat4 GetMatrixY()const abstract;
	virtual glm::dmat4 GetMatrixZ()const abstract;
	virtual glm::dmat4 GetMatrixT()const abstract;
	virtual RigidBody* GetRigidBody() abstract;
	virtual double GetCorrectiveAlpha()const abstract;
	virtual block::Block* GetCollideFront()const abstract;
	virtual block::Block* GetCollideDown()const abstract;
	virtual void ApplyTranslate(const glm::dvec3& offset)abstract;
};