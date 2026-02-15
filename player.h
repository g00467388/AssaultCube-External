#pragma once
#include "entity.h"
#include "vector"
class Player : public Entity
{
private:
	float _yaw;
	float _pitch;
	int _ammo;
	float& getYaw();

	float& getPitch();
	void setYaw(float yaw);
	void setPitch(float pitch);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	using Entity::Entity;
public:
	void aim(Entity& target);

	int32_t getAmmo();
	void setAmmo(uint32_t amount);
	void killAll(std::vector<Entity>& entities);
};
