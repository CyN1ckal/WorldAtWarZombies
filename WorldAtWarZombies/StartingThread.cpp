#include "pch.h"

void CleanUpRoutine(HMODULE hModule, FILE* f)
{
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

DWORD WINAPI StartingThread(HMODULE hModule)
{
	MyImGui::Initialized = false;
	Hook::Initialized = false;
	Config::Initialize();

	FILE* f;

	AllocConsole();

	freopen_s(&f, "CONOUT$", "w", stdout);

	if (!Hook::Hook_DirectX())
	{
		CleanUpRoutine(hModule, f);
		return 0;
	}

	Hook::EnableMiscHooks();

	printf("\n[+] All Hooks Created and Enabled\n");

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}
		Sleep(100);

	}

	printf("[-] Unhooking!\n");

	Hook::Unhook_DirectX();

	Sleep(1000);

	CleanUpRoutine(hModule, f);

	return 0;

}