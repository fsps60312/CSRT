#pragma once
#include<common/visible_object.hpp>
class PodInterface:public VisibleObject {
public:
	virtual bool IsOnGround()const abstract;
	virtual bool IsPodStopped()const abstract;
};