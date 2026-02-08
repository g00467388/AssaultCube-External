#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include <tlhelp32.h>
#include <iostream>
class Mem {
private:
    // ac_client.exe address
    static uintptr_t _moduleBaseAddress; 
    static DWORD _pid;
    Mem() = default;

public:
	static uintptr_t GetModuleBaseAddress(DWORD processID, const char* moduleName);
    static DWORD GetProcessID(const char* processName);
    
};