#pragma once

// Function HOOK prototypes
typedef HRESULT(APIENTRY* EndScene_Template)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(APIENTRY* Reset_Template)(D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(APIENTRY* SetStreamSource_Template)(
	UINT                   StreamNumber,
	IDirect3DVertexBuffer9** ppStreamData,
	UINT* pOffsetInBytes,
	UINT* pStride
	);

class Hook
{
public:
	static HWND window;
	static void* d3d9Device[119];
	static void* EndSceneFunction;
	static void* ResetFunction;
	static void* SetStreamSourceFunction;
	static void* PrintToConsoleFunction;
	static void* PrintToConsoleRawFunction;
	static void* PrintToScreen_MaybeFunction;
	static IDirect3DDevice9* pD3DDevice;
	static int windowHeight, windowWidth;

	static bool Hook_DirectX();
	static void Unhook_DirectX();
	static void EnableMiscHooks();
	
	static HRESULT Reset_Hook(D3DPRESENT_PARAMETERS* pPresentationParameters);
	static HRESULT APIENTRY EndScene_Hook(const LPDIRECT3DDEVICE9 pDevice);
	static void APIENTRY PrintToConsole_Hooked(int OutputBuffer_Maybe, int StringToPrint, ...);

private:
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static BOOL CALLBACK enumWind(const HWND handle, LPARAM lp);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}; 
