#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include "offsets.h"
#include "entity.h"
#include "mem.h"
#include "player.h"
#include <algorithm>
#include <thread>

int32_t GetEntityCount(HANDLE gameHandle, uintptr_t moduleBaseAddress)
{
	uint32_t count_buffer;
	ReadProcessMemory(gameHandle, (LPCVOID)(moduleBaseAddress + offsets::playerCount), &count_buffer, sizeof(count_buffer), nullptr);
	return count_buffer;
}

int main() {
	const char* moduleName = "ac_client.exe";
	uintptr_t pid = Mem::GetProcessID(moduleName);
	DWORD baseAddress = Mem::GetModuleBaseAddress(pid, moduleName);
	if (baseAddress == 0)
	{
		std::cerr << "Failed to get the base address." << std::endl;
		return EXIT_FAILURE;
	}

	uintptr_t localplayer_address = baseAddress + offsets::localplayer;

	HANDLE gameHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
	if (gameHandle == NULL) {
		std::cerr << "Could not open process." << std::endl;
		return EXIT_FAILURE;
	}


	uintptr_t entitylist_buffer;

	uintptr_t entityListBase = baseAddress + offsets::entityList;

	ReadProcessMemory(gameHandle, (LPCVOID)entityListBase, &entitylist_buffer, sizeof(entitylist_buffer), nullptr);

	if (entitylist_buffer == 0)
		throw std::runtime_error("could not get entity list");

	std::vector<Entity> entities;

	for (int i = 0; i <= GetEntityCount(gameHandle, baseAddress); i++) {
		uint32_t entityBaseAddress32 = 0;
		SIZE_T bytesRead = 0;

		if (!ReadProcessMemory(gameHandle, (LPCVOID)(entitylist_buffer + i * 0x4), &entityBaseAddress32, sizeof(entityBaseAddress32), &bytesRead) || bytesRead != sizeof(entityBaseAddress32)) {
			std::cerr << "Failed to read entity pointer at index " << i << " (bytesRead=" << bytesRead << "), GetLastError=" << GetLastError() << "\n";
			continue;
		}
		if (entityBaseAddress32 == 0)
			continue;
		uintptr_t entityBaseAddress = static_cast<uintptr_t>(entityBaseAddress32);
		Entity entity(gameHandle, entityBaseAddress);
		entities.push_back(entity);
	}
	std::cout << "entity count: " << entities.size() << "\n";




	uintptr_t player_buff;

	ReadProcessMemory(gameHandle, (LPCVOID)localplayer_address, &player_buff, sizeof(player_buff), nullptr);

	Player player(gameHandle, player_buff);
	bool unlimitedAmmoEnabled = false, aimbotEnabled = false, killAllEnabled = false;

	aimbotEnabled
		? std::cout << "1) [enabled] Toggle aimbot: " << "\n"
		: std::cout << "1) [disabled] Toggle aimbot: " << "\n";

	unlimitedAmmoEnabled
		? std::cout << "2) [enabled] Increase ammo: " << "\n"
		: std::cout << "2) [disabled] Increase ammo: " << "\n";

	killAllEnabled
		? std::cout << "3) [enabled] Toggle kill all: " << "\n"
		: std::cout << "3) [disabled] Toggle kill all: " << "\n";

	std::cout << "4) Exit\n";
	int input;

	std::cin >> input;

	switch (input)
	{
	case 1:
		player.startAimbot(entities);
		break;
		// ammo
	case 2:
		player.setAmmo(1337);
		break;
	case 3:
		player.killAll(entities);
		break;
	case 4:
		CloseHandle(gameHandle);
		return EXIT_SUCCESS;
	}

	CloseHandle(gameHandle);
	return EXIT_FAILURE;
}

