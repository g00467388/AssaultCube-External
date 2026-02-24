#include <cmath>
#define _USE_MATH_DEFINES

#include <math.h>
#include <windows.h>
#include <thread>
#include "player.h"
#include "offsets.h"

void Player::aim(Entity& target) {
	// Normalize distance from target and player
	float xDelta = target.getX() - this->getX();
	float YDelta = target.getY() - this->getY();
	float ZDelta = target.getZ() - this->getZ();

	float hypotenuse = sqrt(xDelta * xDelta + YDelta * YDelta);

	// atan2 requires radians rather than degrees
	float pitch = atan2(ZDelta, hypotenuse) * (180 / M_PI);
	float yaw = (atan2(YDelta, xDelta)) * (180 / M_PI);
	yaw += 90.f;
	this->setYaw(yaw);
	this->setPitch(pitch);
}

float& Player::getYaw() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_baseAddress + offsets::yaw), &_yaw, sizeof(_yaw), nullptr);
	return _yaw;
}

float& Player::getPitch() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_baseAddress + offsets::pitch), &_pitch, sizeof(_pitch), nullptr);
	return _pitch;
}

void Player::setX(float x)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::Xchord), &x, sizeof(x), nullptr);
}

void Player::setY(float y)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::Ychord), &y, sizeof(y), nullptr);
}

void Player::setZ(float z)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::Zchord), &z, sizeof(z), nullptr);
}

void Player::setYaw(float yaw) {
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::yaw), &yaw, sizeof(yaw), nullptr);
}

void Player::setPitch(float pitch)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::pitch), &pitch, sizeof(pitch), nullptr);
}

void Player::moveTo(Vec3 location)
{
	this->setX(location.x);
	this->setY(location.y);
	this->setZ(location.z);
}

int32_t Player::getAmmo()
{
	ReadProcessMemory(_gameHandle, (LPCVOID)(_baseAddress + offsets::ammo), &_ammo, sizeof(_ammo), nullptr);
	return _ammo;
}

void Player::setAmmo(uint32_t amount)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_baseAddress + offsets::ammo), &amount, sizeof(amount), nullptr);
}

void Player::killAll(std::vector<Entity>& entities)
{
	while (true)
	{
		for (int i = 0; i < entities.size() - 1; i++)
		{
			while (entities[i].getHealth() > 0)
			{
				if (this->getAmmo() == 0)
					this->setAmmo(99);

				// Health is a uint_32, sometimes it underflows instead of hitting 0
				if (entities[i].getHealth() > 100)
					break;

				this->setX(entities[i].getX());
				this->setY(entities[i].getY());
				this->setZ(entities[i].getZ());
				this->aim(entities[i]);

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}
}
