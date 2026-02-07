#pragma once 
#include <windows.h>
class Entity
{
protected:
	HANDLE _gameHandle;
	uintptr_t _playerBaseAddress;
	
	int _health;

	struct {
		float X;
		float Y;
		float Z;
	} _position;
public:
	Entity(HANDLE gamehandle, uintptr_t playerBaseAddress);

	// Getters for member variables
	int getHealth();

	float& getX(); 
	float& getY();
	float& getZ();
};