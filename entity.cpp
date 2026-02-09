#include <iostream>
#include "offsets.h"
#include "entity.h"


float& Entity::getX()  {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::X), &_position.X, sizeof(_position.X), nullptr);
	return _position.X;
}

float& Entity::getY() {
	if (ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::Y), &_position.Y, sizeof(_position.Y), nullptr) == 0)
		throw std::runtime_error("Failed to read player Y position");
	return _position.Y;
}

float& Entity::getZ() {
	if (ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::Z), &_position.Z, sizeof(_position.Z), nullptr) == 0)
		throw std::runtime_error("Failed to read player Z position");
	return _position.Z;
}

uint32_t Entity::getHealth() {
	ReadProcessMemory(_gameHandle, (LPCVOID)(_playerBaseAddress + offsets::health), &_health, sizeof(_health), nullptr);
	return _health;
}