#include "pch.h"

// Global Variables
bool bInit = false;

// Declarations from the static class
HWND Hook::window;
void* Hook::d3d9Device[119];
void* Hook::EndSceneFunction;
void* Hook::ResetFunction;
void* Hook::SetStreamSourceFunction;
void* Hook::PrintToConsoleFunction;
void* Hook::PrintToConsoleRawFunction;
void* Hook::PrintToScreen_MaybeFunction;
IDirect3DDevice9* Hook::pD3DDevice;
int Hook::windowHeight, Hook::windowWidth;

ID3DXFont* Draw::pFont[1];


PrintToScreenMaybe_Template PrintToScreen_Maybe_Original = nullptr;
void APIENTRY PrintToScreen_Maybe_Hooked(int a1, ...)
{
	PrintToScreen_Maybe_Original(a1);
}

PrintRawToConsole_Template PrintRawToConsole_Original = nullptr;
void __cdecl PrintRawToConsole_Hooked(int a1, const char* a2, int a3)
{
	//printf("a1: %d,a2: %s,a3: %d\n", a1,a2,a3);
	PrintRawToConsole_Original(a1, a2, a3);
}

// Print To Console Hook
PrintToConsole_Template PrintToConsole_Original = nullptr;
void APIENTRY Hook::PrintToConsole_Hooked(int OutputBuffer_Maybe, int StringToPrint, ...)
{
	PrintToConsole_Original(OutputBuffer_Maybe, StringToPrint);
}

void Hook::EnableMiscHooks()
{
	PrintToConsoleFunction = (void*)0x59a2c0;
	MH_CreateHook(PrintToConsoleFunction, &PrintToConsole_Hooked, reinterpret_cast<LPVOID*>(&PrintToConsole_Original));
	MH_EnableHook(PrintToConsoleFunction);
	PrintToConsoleRawFunction = (void*)0x59A170;
	MH_CreateHook(PrintToConsoleRawFunction, &PrintRawToConsole_Hooked, reinterpret_cast<LPVOID*>(&PrintRawToConsole_Original));
	MH_EnableHook(PrintToConsoleRawFunction);
	PrintToScreen_MaybeFunction = (void*)0x5f6d80;
	MH_CreateHook(PrintToScreen_MaybeFunction, &PrintToScreen_Maybe_Hooked, reinterpret_cast<LPVOID*>(&PrintToScreen_Maybe_Original));
	MH_EnableHook(PrintToScreen_MaybeFunction);

}



// Setting up EndScene hook
EndScene_Template EndScene_Original = nullptr;
HRESULT APIENTRY Hook::EndScene_Hook(const LPDIRECT3DDEVICE9 pDevice)
{
	if (!bInit)
	{
		pD3DDevice = pDevice;
		bInit = true;
		D3DXCreateFont(pD3DDevice, 24, 8, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Consolas", &Draw::pFont[0]);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		//Hack::PrintRawToConsole(16, "test\n", 0);
		//Hack::PrintErrorToConsole(9, (int)"ERROR: CyNickal\n", "");
		//Hack::PrintToConsole(16, (int)"^1CyNickal Testing: %s\n", "");
		Hack::GetNumZombies();
	}

	//Hack::PrintAliveEnts();

	//IDirect3DStateBlock9* stateBlock = nullptr;
	//IDirect3DPixelShader9* pixelShader = nullptr;
	//IDirect3DBaseTexture9* texture = nullptr;

	//pDevice->GetTexture(0, &texture);
	//pDevice->GetPixelShader(&pixelShader);
	//pDevice->CreateStateBlock(D3DSBT_ALL, &stateBlock);


	if (Hack::Local_Player->Time)
	{
		Draw::DrawZombieTracers(pD3DDevice);

		//Draw::DrawTypeTracers(pD3DDevice, 16);

		Draw::DrawHealthBar(pD3DDevice);

		Draw::DrawZombieCount(pD3DDevice);
	}

	//pDevice->SetTexture(0, texture);
	//pDevice->SetPixelShader(pixelShader);
	//stateBlock->Apply();

	return EndScene_Original(pDevice);
}

// Setting up Reset hook
Reset_Template Reset_Original = nullptr;
HRESULT Hook::Reset_Hook(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	printf("Reset Called!\n");
	return Reset_Original(pPresentationParameters);
}

// Setting up SetStreamSource hook
SetStreamSource_Template GetStreamSource_Original = nullptr;
HRESULT APIENTRY GetStreamSource_Hook(
	UINT                   StreamNumber,
	IDirect3DVertexBuffer9** ppStreamData,
	UINT* pOffsetInBytes,
	UINT* pStride
)
{
	//printf("Set Stream Source Hit!\n");
	//printf("Parameters: %u, %X, %X, %X\n\n", StreamNumber,ppStreamData,pOffsetInBytes,pStride);
	return GetStreamSource_Original(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}


/*
	brief: Creates and initializes the DirectX hooks
*/
bool Hook::Hook_DirectX()
{
	MH_Initialize();

	GetD3D9Device(d3d9Device, sizeof(d3d9Device));

	EndSceneFunction = d3d9Device[42];

	printf("End Scene Address: %X\n", (uintptr_t)EndSceneFunction);

	if (MH_CreateHook(EndSceneFunction, &EndScene_Hook, reinterpret_cast<LPVOID*>(&EndScene_Original)) != MH_OK)
	{
		printf("Error Creating EndSceneFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	if (MH_EnableHook(EndSceneFunction) != MH_OK)
	{
		printf("Error Enabling EndSceneFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	ResetFunction = d3d9Device[16];

	printf("Reset Address: %X\n", (uintptr_t)ResetFunction);

	if (MH_CreateHook(ResetFunction, &Reset_Hook, reinterpret_cast<LPVOID*>(&Reset_Original)) != MH_OK)
	{
		printf("Error Creating ResetFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	if (MH_EnableHook(ResetFunction) != MH_OK)
	{
		printf("Error Enabling ResetFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	SetStreamSourceFunction = d3d9Device[100];

	printf("SetStreamSourceFunction Address: %X\n", (uintptr_t)SetStreamSourceFunction);

	if (MH_CreateHook(SetStreamSourceFunction, &GetStreamSource_Hook, reinterpret_cast<LPVOID*>(&GetStreamSource_Original)) != MH_OK)
	{
		printf("Error Creating SetStreamSourceFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	if (MH_EnableHook(SetStreamSourceFunction) != MH_OK)
	{
		printf("Error Enabling SetStreamSourceFunction Hook! Exiting.\n");
		Unhook_DirectX();
		return 0;
	}

	return 1;
}

/*
	brief: Disables previous DirectX hooks
*/
void Hook::Unhook_DirectX()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}


/*
	brief: Callback function for the enumerate windows function
*/
BOOL CALLBACK Hook::enumWind(const HWND handle, LPARAM lp)
{
	DWORD procID;
	GetWindowThreadProcessId(handle, &procID);
	if (GetCurrentProcessId() != procID)
		return TRUE;

	window = handle;
	return FALSE;
}

/*
	brief: Return the current process' HWND
*/
HWND Hook::GetProcessWindow()
{
	window = nullptr;

	EnumWindows(enumWind, NULL);

	RECT size;
	if (window == nullptr)
		return nullptr;

	GetWindowRect(window, &size);

	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

/*
	brief: Getting D3D9Device using a dummy device
*/
BOOL Hook::GetD3D9Device(void** pTable, const size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();
	d3dpp.Windowed = (GetWindowLongPtr(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0 ? FALSE : TRUE;

	if (HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice); dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return FALSE;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return TRUE;
}



