#pragma once
#include "entity.h"

class Player : public Entity
{
private:
	float _yaw;
	float _pitch;
	float& getYaw();
	float& getPitch();

public:
	void aim(Entity& target);
};