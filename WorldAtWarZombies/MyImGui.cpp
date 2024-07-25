#include "pch.h"

bool Config::VerticalLineESP;

bool MyImGui::ShowMyWindow() {
  ImGui::SetNextWindowPos({50, 100}, ImGuiCond_FirstUseEver);

  ImGui::Begin("Visuals", 0, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Zombie Tracers", &Config::TracerLines);
  ImGui::Checkbox("Zombie Vertical Line", &Config::VerticalLineESP);
  ImGui::Checkbox("Health Bar", &Config::LocalPlayerHealthBar);
  ImGui::Checkbox("Zombie Count", &Config::ZombieCount);
  ImGui::Checkbox("Type Tracers", &Config::TypeTracers);
  ImGui::InputInt("Type Number", &Config::TypeNumber);
  ImGui::Checkbox("Debug Visuals", &Config::DebugVisuals);

  ImGui::End();

  ImGui::SetNextWindowPos({400, 100}, ImGuiCond_FirstUseEver);

  ImGui::Begin("Combat",0, ImGuiWindowFlags_AlwaysAutoResize);

  if (ImGui::Button("Toggle Infinite Ammo")) {
    Hack::ToggleInfiniteAmmo(!Config::InfiniteAmmo);
  }

  ImGui::End();

  return true;
}

bool MyImGui::Initialize(LPDIRECT3DDEVICE9 pD3DDevice) {
  D3DDEVICE_CREATION_PARAMETERS CP;
  pD3DDevice->GetCreationParameters(&CP);
  HWND window = CP.hFocusWindow;
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.IniFilename = nullptr;
  io.Fonts->AddFontDefault();
  ImGui::StyleColorsDark();
  ImGui_ImplWin32_Init(window);
  ImGui_ImplDX9_Init(pD3DDevice);
  MyImGui::Initialized = true;

  return true;
}