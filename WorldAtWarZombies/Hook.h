#pragma once

class Hook
{
public:
	static HWND window;
	static void* d3d9Device[119];
	static void* EndSceneFunction;
	static void* ResetFunction;
	static void* SetStreamSourceFunction;
	static IDirect3DDevice9* pD3DDevice;
	static int windowHeight, windowWidth;
	static HRESULT APIENTRY EndScene_Hook(const LPDIRECT3DDEVICE9 pDevice);
	static HRESULT Reset_Hook(D3DPRESENT_PARAMETERS* pPresentationParameters);
	static void Hook_DirectX();
	static void Unhook_DirectX();
private:
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static BOOL CALLBACK enumWind(const HWND handle, LPARAM lp);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}; 
