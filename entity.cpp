#include <iostream>
#include "offsets.h"
#include "entity.h"

Entity::Entity(HANDLE& gameHandle, uintptr_t playerBaseAddress) : _gameHandle(gameHandle), _playerBaseAddress(playerBaseAddress) {}

float Entity::getX() const  {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::X), (void*)&_position.X, sizeof(_position.X), nullptr);
	return _position.X;
}

float Entity::getY() const {
	if (ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::Y), (void*)&_position.Y, sizeof(_position.Y), nullptr) == 0)
		throw std::runtime_error("Failed to read player Y position");
	return _position.Y;
}

float Entity::getZ() const {
	if (ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::Z), (void*)&_position.Z, sizeof(_position.Z), nullptr) == 0)
		throw std::runtime_error("Failed to read player Z position");
	return _position.Z;
}

uint32_t Entity::getHealth() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::health), &_health, sizeof(_health), nullptr);
	return _health;
}

void Entity::calculateDistance(Entity& localplayer)  {
	_distance = sqrt(std::pow(this->getX() - localplayer._position.X, 2) +
		std::pow(this->getY() - localplayer._position.Y, 2) +
		std::pow(this->getZ() - localplayer._position.Z, 2)
	);

}

bool Entity::operator<(const Entity& other)
{
	return this->_distance < other._distance;
}

bool Entity::operator>(const Entity& other)
{
	return this->_distance > other._distance;
}

bool Entity::operator==(const Entity& other)
{
	return this->_distance == other._distance;
}

