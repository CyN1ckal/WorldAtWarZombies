#include "pch.h"

// Declarations from the static classes

bool Config::TracerLines;
bool Config::MasterImgui;
bool Config::LocalPlayerHealthBar;
bool Config::ZombieCount;
bool Config::TypeTracers;
int Config::TypeNumber;
bool Config::InfiniteAmmo;
bool Config::DebugVisuals;
bool Config::SilentReload;

bool MyImGui::Initialized;

bool Hook::Initialized;
HWND Hook::window;
void *Hook::d3d9Device[119];
void *Hook::EndSceneFunction;
void *Hook::ResetFunction;
void *Hook::SetStreamSourceFunction;
void *Hook::BeginSceneFunction;
void *Hook::PrintToConsoleFunction;
void *Hook::PrintRawToConsoleFunction;
void *Hook::PrintToScreen_MaybeFunction;
void *Hook::WndProcFunction;
IDirect3DDevice9 *Hook::pD3DDevice;
int Hook::windowHeight, Hook::windowWidth;
int Hook::PreviousWindowHeight;
int Hook::PreviousWindowWidth;

ID3DXFont *Draw::pFont[3];

extern bool PerfDrawInit;
extern IDirect3DTexture9 *Primitive;
/*
    brief: BeginScene hook needed for getting texture
*/
BeginScene_Template BeginScene_Original = nullptr;
HRESULT __stdcall Hook::BeginScene_Hooked(LPDIRECT3DDEVICE9 m_pD3Ddev) {
  if (!PerfDrawInit && Hook::Initialized) {
    Draw::GenerateTexture(pD3DDevice, &Primitive,
                          D3DCOLOR_ARGB(255, 255, 255, 255));
    PerfDrawInit = true;
  }
  return BeginScene_Original(m_pD3Ddev);
}

/*
    brief: Hooked Window procedure. Needed for ImGui implementation

    I have since realized that I do not need to hook WndProc with MinHook, and
   instead I can use the WinAPI SetWindowLongPtr to redirect the WndProc to my
   own. But since it aint broke; I aint fixing it.
*/
WndProc_Template WndProc_Original = nullptr;
int __stdcall Hook::WndProc_Hooked(HWND hWnd, UINT Msg, int wParam,
                                   LPARAM lParam) {
  extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
      HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam)) {
    ImGui::GetIO().MouseDrawCursor =
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    return true;
  }

  if (MyImGui::Initialized)
    ImGui::GetIO().MouseDrawCursor =
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

  if (ImGui::GetIO().WantCaptureMouse) {
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
      return true;
    return false;
  }

  return WndProc_Original(hWnd, Msg, wParam, lParam);
}

/*
        brief: This is the print to console function but after the string is
   already formatted. This is the "raw" string.
*/
PrintRawToConsole_Template PrintRawToConsole_Original = nullptr;
void __cdecl Hook::PrintRawToConsole_Hooked(int a1, const char *a2, int a3) {
  // printf("a1: %d,a2: %s,a3: %d\n", a1,a2,a3);
  PrintRawToConsole_Original(a1, a2, a3);
}

/*
        brief: Print text to the in-game console. I was using this to dump the
         cmd, but currently not being used. Formatting works, and you can also
        color-code by using ^0 - ^6 as a prefix.
*/
PrintToConsole_Template PrintToConsole_Original = nullptr;
void APIENTRY Hook::PrintToConsole_Hooked(int OutputBuffer_Maybe,
                                          int StringToPrint, ...) {
  PrintToConsole_Original(OutputBuffer_Maybe, StringToPrint);
}

/*
        brief: The main EndScene hook. This is the hook which is called every
   frame, so this is where I do the hack logic as well.
*/
int AimbotDelayInt = 0;
EndScene_Template EndScene_Original = nullptr;
HRESULT APIENTRY Hook::EndScene_Hook(const LPDIRECT3DDEVICE9 pDevice) {
  if (!Hook::Initialized) {
    pD3DDevice = pDevice;
    Draw::CreateFonts(pD3DDevice);
    Hook::Initialized = true;
  }

  if (!MyImGui::Initialized) {
    MyImGui::Initialize(pDevice);
  }

  if (GetAsyncKeyState(VK_INSERT) & 1) {
    Config::MasterImgui = !Config::MasterImgui;
  }

  /*
    brief: ImGui implementation area
  */
  ImGui_ImplDX9_NewFrame();
  ImGui_ImplWin32_NewFrame();

  ImGui::NewFrame();

  if (Config::MasterImgui) {
    MyImGui::UpdateColors();
    MyImGui::ShowMyWindows();
  }

  ImGui::EndFrame();
  /*
    End of ImGui implementation area
  */

  if (Config::DebugVisuals) {
    Debug::PrintFacingDirection();
    Debug::PrintPlayerCoordinates();
  }

  if (Hack::Local_Player->Time) {

    Hack::FillZombieVector();

    if (Config::SilentReload)
      Hack::SilentReload();

    if (Config::VerticalLineESP)
      Draw::VerticalLineESP(pD3DDevice);

    if (Config::TracerLines)
      Draw::DrawZombieTracers(pD3DDevice, DrawTracerType::Origin);

    if (Config::TypeTracers)
      Draw::DrawTypeTracers(pD3DDevice, (EntityType)Config::TypeNumber);

    if (Config::LocalPlayerHealthBar)
      Draw::DrawHealthBar(pD3DDevice);

    if (Config::ZombieCount)
      Draw::DrawZombieCount(pD3DDevice);

    if (Config::InfiniteAmmo)
      Draw::InfiniteAmmoText(pD3DDevice);

    if (GetAsyncKeyState(VK_XBUTTON2) && AimbotDelayInt % 2 == 0) {
      Hack::AimAtClosestZombieHead();
    }
    AimbotDelayInt++;
  }

  Draw::Watermark();

  /*
    Render ImGui here instead of directly after the end of the implementation
    area so the ESP draws under the menu
  */
  ImGui::Render();
  ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

  return EndScene_Original(pDevice);
}

/*
        brief: Reset hook. It honestly does nothing; it seems to never get
   called. Usually this is what gets called when you resize the screen, but
   because COD dosent let you, it is basically useless.
*/
Reset_Template Reset_Original = nullptr;
HRESULT Hook::Reset_Hook(D3DPRESENT_PARAMETERS *pPresentationParameters) {
  printf("Reset Called!\n");
  return Reset_Original(pPresentationParameters);
}

/*
        brief: SetStreamSource hook. I was using this to probe what parameters
   the game is passing. Not really being used right now.
*/
SetStreamSource_Template GetStreamSource_Original = nullptr;
HRESULT APIENTRY Hook::GetStreamSource_Hook(
    UINT StreamNumber, IDirect3DVertexBuffer9 **ppStreamData,
    UINT *pOffsetInBytes, UINT *pStride) {
  return GetStreamSource_Original(StreamNumber, ppStreamData, pOffsetInBytes,
                                  pStride);
}

/*
    brief: Reload function hook. Dumping parameters.
*/
Reload_Template Reload_Original = nullptr;
int __stdcall Hook::Reload_Hooked() { return Reload_Original(); }


/*
    brief: Shoot hook. Needed to hook it because I am trying to call it... and failing...
*/
Shoot_Template Shoot_Original = nullptr;
char __cdecl Hook::Shoot_Hooked(int LocalPlayer, int One, int WeaponPtrArrayIndex) {


  return Shoot_Original(LocalPlayer, One, WeaponPtrArrayIndex);
}

ShootWrapper_Template ShootWrapper_Original = nullptr;
void** __cdecl ShootWrapper_Hooked(int a1, int a2)
{
  std::cout << a1 << std::endl;
  std::cout << std::hex << a2 << std::endl;
  return ShootWrapper_Original(a1, a2);
}

/*
        brief: Enabling hooks which arent DirectX
*/
void *Hook::Reload_Function;
void *Hook::Shoot_Function;
void *Hook::ShootWrapper_Function;
void Hook::EnableMiscHooks() {
  PrintToConsoleFunction = (void *)(Hack::WaW_BaseAddress + Offsets::PrintToConsoleOffset);
  MH_CreateHook(PrintToConsoleFunction, &PrintToConsole_Hooked,
                reinterpret_cast<LPVOID *>(&PrintToConsole_Original));
  MH_EnableHook(PrintToConsoleFunction);

  PrintRawToConsoleFunction = (void *)(Hack::WaW_BaseAddress + Offsets::PrintRawToConsoleOffset);
  MH_CreateHook(PrintRawToConsoleFunction, &PrintRawToConsole_Hooked,
                reinterpret_cast<LPVOID *>(&PrintRawToConsole_Original));
  MH_EnableHook(PrintRawToConsoleFunction);

  WndProcFunction = (void *)(Hack::WaW_BaseAddress + Offsets::WndProcOffset);
  MH_CreateHook(WndProcFunction, &WndProc_Hooked,
                reinterpret_cast<LPVOID *>(&WndProc_Original));
  MH_EnableHook(WndProcFunction);

  Reload_Function = (void *)(Hack::WaW_BaseAddress + Offsets::ReloadMaybeOffset);
  MH_CreateHook(Reload_Function, &Reload_Hooked,
                reinterpret_cast<LPVOID *>(&Reload_Original));
  MH_EnableHook(Reload_Function);

  Shoot_Function = (void *)(Hack::WaW_BaseAddress + Offsets::ShootOffset);
  MH_CreateHook(Shoot_Function, &Shoot_Hooked,
                reinterpret_cast<LPVOID *>(&Shoot_Original));
  MH_EnableHook(Shoot_Function);

  ShootWrapper_Function = (void *)(Hack::WaW_BaseAddress + 0x20A60);
  MH_CreateHook(ShootWrapper_Function, &ShootWrapper_Hooked,
                reinterpret_cast<LPVOID *>(&ShootWrapper_Original));
  MH_EnableHook(ShootWrapper_Function);
}


/*
        brief: Creates and initializes the DirectX hooks
*/
bool Hook::Hook_DirectX() {
  MH_Initialize();

  GetD3D9Device(d3d9Device, sizeof(d3d9Device));

  EndSceneFunction = d3d9Device[42];

  printf("End Scene Address: %X\n", (uintptr_t)EndSceneFunction);

  if (MH_CreateHook(EndSceneFunction, &EndScene_Hook,
                    reinterpret_cast<LPVOID *>(&EndScene_Original)) != MH_OK) {
    printf("Error Creating EndSceneFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  if (MH_EnableHook(EndSceneFunction) != MH_OK) {
    printf("Error Enabling EndSceneFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  ResetFunction = d3d9Device[16];

  printf("Reset Address: %X\n", (uintptr_t)ResetFunction);

  if (MH_CreateHook(ResetFunction, &Reset_Hook,
                    reinterpret_cast<LPVOID *>(&Reset_Original)) != MH_OK) {
    printf("Error Creating ResetFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  if (MH_EnableHook(ResetFunction) != MH_OK) {
    printf("Error Enabling ResetFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  SetStreamSourceFunction = d3d9Device[100];

  printf("SetStreamSourceFunction Address: %X\n",
         (uintptr_t)SetStreamSourceFunction);

  if (MH_CreateHook(SetStreamSourceFunction, &GetStreamSource_Hook,
                    reinterpret_cast<LPVOID *>(&GetStreamSource_Original)) !=
      MH_OK) {
    printf("Error Creating SetStreamSourceFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  if (MH_EnableHook(SetStreamSourceFunction) != MH_OK) {
    printf("Error Enabling SetStreamSourceFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  BeginSceneFunction = d3d9Device[41];

  printf("BeginSceneFunction Address: %X\n", (uintptr_t)BeginSceneFunction);

  if (MH_CreateHook(BeginSceneFunction, &BeginScene_Hooked,
                    reinterpret_cast<LPVOID *>(&BeginScene_Original)) !=
      MH_OK) {
    printf("Error Creating BeginSceneFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  if (MH_EnableHook(BeginSceneFunction) != MH_OK) {
    printf("Error Enabling BeginSceneFunction Hook! Exiting.\n");
    Unhook_DirectX();
    return 0;
  }

  return 1;
}

/*
        brief: Disables previous DirectX hooks
*/
void Hook::Unhook_DirectX() {
  MH_DisableHook(MH_ALL_HOOKS);
  MH_Uninitialize();
  Draw::pFont[0]->Release();
  Draw::pFont[1]->Release();
  Draw::pFont[2]->Release();
  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

/*
        brief: Callback function for the enumerate windows function
*/
BOOL CALLBACK Hook::enumWind(const HWND handle, LPARAM lp) {
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
HWND Hook::GetProcessWindow() {
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
BOOL Hook::GetD3D9Device(void **pTable, const size_t size) {
  if (!pTable)
    return FALSE;

  IDirect3D9 *pD3D = Direct3DCreate9(D3D_SDK_VERSION);
  if (!pD3D)
    return FALSE;

  IDirect3DDevice9 *pDummyDevice = nullptr;

  D3DPRESENT_PARAMETERS d3dpp = {};
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = GetProcessWindow();
  d3dpp.Windowed =
      (GetWindowLongPtr(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0 ? FALSE
                                                                         : TRUE;

  if (HRESULT dummyDevCreated = pD3D->CreateDevice(
          D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
          D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
      dummyDevCreated != S_OK) {
    d3dpp.Windowed = !d3dpp.Windowed;
    dummyDevCreated = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

    if (dummyDevCreated != S_OK) {
      pD3D->Release();
      return FALSE;
    }
  }

  memcpy(pTable, *(void ***)(pDummyDevice), size);
  pDummyDevice->Release();
  pD3D->Release();
  return TRUE;
}
