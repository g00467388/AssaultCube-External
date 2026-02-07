#include <cstdint>
namespace offsets
{
	static const uintptr_t localplayer = 0x0017E254; // uintptr_t
	static const uintptr_t health = 0xEC; //int
	static const uintptr_t yaw = 0x34; //float
	static const uintptr_t pitch = 0x38; //float
	static const uintptr_t X = 0x4; //float
	static const uintptr_t Y = 0x8; //float 
	static const uintptr_t Z = 0xC; //float 

	// Entity list only contains enemies (not local player)
	// Each entity if offset by 0x4 
	static const uintptr_t entityList = 0x0018AC04; // uintptr_t
}