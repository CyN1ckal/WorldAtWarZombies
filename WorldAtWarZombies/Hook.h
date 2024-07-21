#pragma once

/*
	brief: Function typedefs for hooking and calling game functions
*/
typedef HRESULT(APIENTRY* EndScene_Template)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(APIENTRY* Reset_Template)(D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(APIENTRY* SetStreamSource_Template)(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride);
typedef int(APIENTRY* PrintToConsole_Template)(int a1, int a2, ...);
typedef void(APIENTRY* PrintErrorToConsole_Template)(int a1, int a2, ...);
typedef void(__cdecl* PrintRawToConsole_Template)(int a1, const char* a2, int a3);
typedef int(__stdcall* WndProc_Template)(HWND hWnd, UINT Msg, int wParam, LPARAM lParam);


/*
	brief: Hook class containing all the variables and functions related to hooking
*/
class Hook
{
public:
	// Variables used in hooking
	static HWND window;
	static void* d3d9Device[119];
	static void* EndSceneFunction;
	static void* ResetFunction;
	static void* SetStreamSourceFunction;
	static void* PrintToConsoleFunction;
	static void* PrintRawToConsoleFunction;
	static void* PrintToScreen_MaybeFunction;
	static void* WndProcFunction;
	static IDirect3DDevice9* pD3DDevice;
	static int windowHeight, windowWidth;

	// Hook enable/disable
	static bool Hook_DirectX();
	static void Unhook_DirectX();
	static void EnableMiscHooks();

	// Hooked game function declarations
	static void __cdecl PrintRawToConsole_Hooked(int a1, const char* a2, int a3);
	static void APIENTRY PrintToConsole_Hooked(int OutputBuffer_Maybe, int StringToPrint, ...);
	static HRESULT APIENTRY EndScene_Hook(const LPDIRECT3DDEVICE9 pDevice);
	static HRESULT Reset_Hook(D3DPRESENT_PARAMETERS* pPresentationParameters);
	static HRESULT APIENTRY GetStreamSource_Hook(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride);
	static int __stdcall WndProc_Hooked(HWND hWnd, UINT Msg, int wParam, LPARAM lParam);

	// "Helper" Functions. Not exactly hooking but used in class.
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static BOOL CALLBACK enumWind(const HWND handle, LPARAM lp);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

