#pragma once 
#include <cstdint>
#include <windows.h>
class Entity
{
private:
	double _distance;
protected:
	HANDLE _gameHandle;
	uintptr_t _baseAddress;
	uint32_t _health;
	bool _isVisible;
	struct {
		float X;
		float Y;
		float Z;
	} _position;

public:
	// Get distance between entity and local player
	void calculateDistance(Entity& localplayer);
	Entity(HANDLE& gamehandle, uintptr_t playerBaseAddress);
	uint32_t getHealth();

	// Compares distance between entity and local player (so we can sort by distance using std::sort)
	bool operator<(const Entity& other);
	bool operator>(const Entity& other);
	bool operator==(const Entity& other);
	bool getVisible();
	float getX() const;
	float getY() const;
	float getZ() const;
};