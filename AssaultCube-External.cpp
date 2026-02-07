#include <iostream>
#include <windows.h>
#include <psapi.h>
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
    DWORD processID = 7576;
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

    HANDLE gameHandle = OpenProcess(PROCESS_VM_READ, FALSE, processID);
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

}
