#include "pch.h"

void CleanUpRoutine(HMODULE hModule, FILE* f)
{
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

DWORD WINAPI StartingThread(HMODULE hModule)
{
	FILE* f;

	AllocConsole();

	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Hello from DLL!\n");
		
	Hook::Hook_DirectX();

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}
		Sleep(100);
	}

	Hook::Unhook_DirectX();
	
	CleanUpRoutine(hModule, f);

	return 0;

}