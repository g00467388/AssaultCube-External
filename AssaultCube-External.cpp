#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include "offsets.h"
#include "entity.h"
#include "mem.h"

#define ENTITY_COUNT 4

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
	for (int i = 0; i < ENTITY_COUNT; i++) {
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
	std::cout << entities.size() << "\n";
	for (int i = 0; i < entities.size() ; i++) {
		std::cout << "Entity " << i << " health: " << entities[i].getHealth() << "\n";
		std::cout << "Entity " << i << " position: (" << entities[i].getX() << ", " << entities[i].getY() << ", " << entities[i].getZ() << ")" << "\n\n";
	}
	CloseHandle(gameHandle);
	return EXIT_SUCCESS;


}
