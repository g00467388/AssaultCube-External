#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include "mem.h"

uintptr_t Mem::_moduleBaseAddress = 0;
DWORD Mem::_pid = 0;

uintptr_t Mem::GetModuleBaseAddress(DWORD processID, const char* moduleName) {
    if (_moduleBaseAddress != 0)
		return _moduleBaseAddress;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess) {
        HMODULE hModules[1024];
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
                char currentModuleName[MAX_PATH];
                if (GetModuleFileNameExA(hProcess, hModules[i], currentModuleName, sizeof(currentModuleName) / sizeof(char))) {
                    if (strstr(currentModuleName, moduleName) != nullptr) {
                        _moduleBaseAddress = (uintptr_t)hModules[i];
                        break;
                    }
                }
            }
        }
        CloseHandle(hProcess);
    }
    else {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
    }

    return _moduleBaseAddress;
}
DWORD  Mem::GetProcessID(const char* processName) {
    if (_pid != 0)
        return _pid;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &processEntry)) {
            do {
                if (strcmp(reinterpret_cast<const char*>(processEntry.szExeFile), processName) == 0) {
                    _pid = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
        CloseHandle(hSnapshot);
    }
    else {
        std::cerr << "Failed to create snapshot. Error: " << GetLastError() << std::endl;
    }

    return _pid;
}

