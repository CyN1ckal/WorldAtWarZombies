#include "pch.h"

bool
MyImGui::ShowMyWindow()
{
  ImGui::Begin("Visuals");
  ImGui::Checkbox("Zombie Tracers", &Config::TracerLines);
  ImGui::Checkbox("Health Bar", &Config::LocalPlayerHealthBar);
  ImGui::Checkbox("Zombie Count", &Config::ZombieCount);
  ImGui::Checkbox("Type Tracers", &Config::TypeTracers);
  ImGui::InputInt("Type Number", &Config::TypeNumber);

  if (ImGui::Button("Toggle Infinite Ammo")) {
    Hack::ToggleInfiniteAmmo(!Config::InfiniteAmmo);
  }

  if (ImGui::Button("Dev Button"))
  {
    Hack::ResetViewAngles();
  }

  ImGui::End();

  return true;
}

bool
MyImGui::Initialize(LPDIRECT3DDEVICE9 pD3DDevice)
{
  D3DDEVICE_CREATION_PARAMETERS CP;
  pD3DDevice->GetCreationParameters(&CP);
  HWND window = CP.hFocusWindow;
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.IniFilename = nullptr;
  io.Fonts->AddFontDefault();
  ImGui::StyleColorsDark();
  ImGui_ImplWin32_Init(window);
  ImGui_ImplDX9_Init(pD3DDevice);
  MyImGui::Initialized = true;

  return true;
}