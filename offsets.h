#include <cstdint>
namespace offsets
{
	static const uintptr_t localplayer = 0x0017E0A8; // uintptr_t
	static const uintptr_t health = 0xEC; // uint32_t
	static const uintptr_t yaw = 0x34; //float
	static const uintptr_t pitch = 0x38; //float
	static const uintptr_t ammo = 0x140; //int32_t

	// entity head coordinates
	static const uintptr_t X = 0x4; //float
	static const uintptr_t Y = 0x8; //float 
	static const uintptr_t Z = 0xC; //float 
	//static const uintptr_t IsVisible = 0x3F9; // bool
	static const uintptr_t IsVisible = 0x3B; // bool

	// Local player coordinates
	static const uintptr_t Xchord = 0x28; //float 
	static const uintptr_t Ychord = 0x2c; //float 
	static const uintptr_t Zchord = 0x30; //float 


	// Entity list only contains enemies (not local player)
	// Each entity is offset by 0x4 
	static const uintptr_t entityList = 0x0018AC04; // uintptr_t
	static const uintptr_t playerCount = 0x191FD4; // int32_t
	static const uintptr_t ViewMatrix = 0x57DFFC; // float

}