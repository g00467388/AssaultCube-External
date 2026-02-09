#pragma once
#include "entity.h"

class Player : public Entity
{
private:
	float _yaw;
	float _pitch;
	float& getYaw();
	float& getPitch();
	using Entity::Entity;
public:
	void aim(Entity& target);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
};
