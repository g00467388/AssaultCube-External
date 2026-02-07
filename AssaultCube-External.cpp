#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include "offsets.h"

#define ENTITY_COUNT 4


DWORD GetModuleBaseAddress(DWORD processID, const char* moduleName) {
	DWORD baseAddress = 0;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (hProcess) {
		HMODULE hMods[1024];
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
				char szModName[MAX_PATH];
				if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char))) {
					if (strstr(szModName, moduleName) != nullptr) {
						baseAddress = (DWORD)hMods[i];
						break;
					}
				}
			}
		}
		CloseHandle(hProcess);
	}
	return baseAddress;
}

int main() {
	DWORD processID = 24252;
	const char* moduleName = "ac_client.exe";

	DWORD baseAddress = GetModuleBaseAddress(processID, moduleName);
	if (baseAddress != 0) {
		std::cout << "Base address of " << moduleName << ": " << std::hex << baseAddress << std::endl;
	}
	else {
		std::cerr << "Failed to get the base address." << std::endl;
		return EXIT_FAILURE;
	}

	uintptr_t localplayer_address = baseAddress + offsets::localplayer;

	HANDLE gameHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID);
	if (gameHandle == NULL) {
		std::cerr << "Could not open process." << std::endl;
		return EXIT_FAILURE;
	}


	uintptr_t buffer;

	uintptr_t entityListBase = baseAddress + offsets::entityList;

	ReadProcessMemory(gameHandle, (LPCVOID)entityListBase, &buffer, sizeof(buffer), nullptr);

	if (buffer == 0)
		throw std::runtime_error("could not get entity list");

	std::cout << "entitylistbase: " << buffer << "\n";

	uintptr_t localplayer_buffer;
	float pitch_buffer{};
	float pitch = 0.0f;
	while (true)
	{

		ReadProcessMemory(gameHandle, (LPCVOID)(baseAddress + offsets::localplayer), &localplayer_buffer, sizeof(localplayer_buffer), nullptr);
		//std::cout << "localplayer buffer: " << localplayer_buffer << "\n";
		ReadProcessMemory(gameHandle, (LPCVOID)(localplayer_buffer + offsets::pitch), &pitch_buffer, sizeof(pitch_buffer), nullptr);
		std::cout << "\rpitch buffer: " << pitch_buffer;
		WriteProcessMemory(gameHandle, (LPVOID)(localplayer_buffer + offsets::pitch), &pitch, sizeof(pitch_buffer), nullptr);
		//std::cout << "pitch buffer: " << pitch_buffer << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	CloseHandle(gameHandle);
	return EXIT_SUCCESS;


}
