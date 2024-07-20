#pragma once

// Function CALL and HOOK prototypes
typedef int(APIENTRY* PrintToConsole_Template)(int a1, int a2, ...);
typedef void(APIENTRY* PrintErrorToConsole_Template)(int a1, int a2, ...);
typedef void(__cdecl* PrintRawToConsole_Template)(int a1, const char* a2, int a3);
typedef int(APIENTRY* PrintToScreenMaybe_Template)(int a1, ...);

class Hack
{
public:

	static inline PrintToConsole_Template PrintToConsole = reinterpret_cast<PrintToConsole_Template>(0x59A2C0);
	static inline PrintErrorToConsole_Template PrintErrorToConsole = reinterpret_cast<PrintErrorToConsole_Template>(0x59a380);
	static inline PrintRawToConsole_Template PrintRawToConsole = reinterpret_cast<PrintRawToConsole_Template>(0x59A170);
	static inline PrintToScreenMaybe_Template PrintToScreen_Maybe = reinterpret_cast<PrintToScreenMaybe_Template>(0x5f6d80);

	static inline uintptr_t WaW_BaseAddress = (uintptr_t)GetModuleHandle(NULL);

private:
};