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
	static inline refdef_t* RefDef = reinterpret_cast<refdef_t*>(WaW_BaseAddress + 0x3120338);
	static inline LocalPlayer* Local_Player = (LocalPlayer*)(WaW_BaseAddress + 0x136c6f0);
	static inline auto* pViewMatrix = (float*)(0x008E870C);

	static int GetNumZombies();
private:
};