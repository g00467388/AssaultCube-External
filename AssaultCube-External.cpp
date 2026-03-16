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
	if (baseAddress != 0) {
		std::cout << "Base address of " << moduleName << ": " << std::hex << baseAddress << std::endl;
	}
	else {
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
	std::cout << "entitylistbase: " << entitylist_buffer << "\n";

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
		std::cout << "Entity " << i << " base address: 0x" << std::hex << entityBaseAddress << std::dec << "\n";
	}
	std::cout << "entity count: " << entities.size() << "\n";




	uintptr_t player_buff;

	ReadProcessMemory(gameHandle, (LPCVOID)localplayer_address, &player_buff, sizeof(player_buff), nullptr);

	std::cout << player_buff << "\n";
	Player player(gameHandle, player_buff);
	int index = 0;


    while (true)
    {
        if (entities.empty()) {
            continue;
        }

        for (auto &e : entities) {	
            e.calculateDistance(player);
        }	

		std::sort(entities.begin(), entities.end());
		

		if (entities[index].getHealth() <= 0 || entities[index].getHealth() > 100)
			index++;
		else if (entities[0].getHealth() > 0 && entities[0].getHealth() <= 100)	
			index = 0;


        player.aim(entities[index]);
		player.setAmmo(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
	CloseHandle(gameHandle);
}

