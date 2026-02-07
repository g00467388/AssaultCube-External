#include "player.h"
#include "offsets.h"

void Player::aim(Entity& target) {

}

float& Player::getYaw() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::yaw), &_yaw, sizeof(_yaw), nullptr);
	return _yaw;
}

float& Player::getPitch() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::pitch), &_pitch, sizeof(_pitch), nullptr);
	return _pitch;
}