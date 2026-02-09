#pragma once 
#include <cstdint>
#include <windows.h>
class Entity
{
protected:
	HANDLE _gameHandle;
	uintptr_t _playerBaseAddress;
	
	uint32_t _health;

	struct {
		float X;
		float Y;
		float Z;
	} _position;
public:
	Entity(HANDLE& gamehandle, uintptr_t playerBaseAddress);

	// Getters for member variables
	uint32_t getHealth();

	float& getX(); 
	float& getY();
	float& getZ();
};