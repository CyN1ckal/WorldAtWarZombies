#pragma once

class Hook
{
public:
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
