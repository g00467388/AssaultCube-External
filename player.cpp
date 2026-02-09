#include <windows.h>
#include "player.h"
#include "offsets.h"
Entity::Entity(HANDLE& gameHandle, uintptr_t playerBaseAddress) : _gameHandle(gameHandle), _playerBaseAddress(playerBaseAddress) {
}
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
void Player::setX(float x)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_playerBaseAddress + offsets::Xchord), &x, sizeof(x), nullptr);
}

void Player::setY(float y)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_playerBaseAddress + offsets::Ychord), &y, sizeof(y), nullptr);
}

void Player::setZ(float z)
{
	WriteProcessMemory(_gameHandle, (LPVOID)(_playerBaseAddress + offsets::Zchord), &z, sizeof(z), nullptr);
}
