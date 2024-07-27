#include "pch.h"

bool Config::VerticalLineESP;

/*
    brief: Updates the RGB_A colors from the ImVec4. Called when the menu is
   open.
*/
bool MyImGui::UpdateColors() {
  MyImGui::MenuThemeColor_RGBA = {
      static_cast<int>(MyImGui::MenuThemeColor.x * 255),
      static_cast<int>(MyImGui::MenuThemeColor.y * 255),
      static_cast<int>(MyImGui::MenuThemeColor.z * 255),
      static_cast<int>(MyImGui::MenuThemeColor.w * 255)};

  MyImGui::ZombieTracerColor_RGBA = {
      static_cast<int>(MyImGui::ZombieTracerColor.x * 255),
      static_cast<int>(MyImGui::ZombieTracerColor.y * 255),
      static_cast<int>(MyImGui::ZombieTracerColor.z * 255),
      static_cast<int>(MyImGui::ZombieTracerColor.w * 255)};

  MyImGui::ZombieVerticalLineColor_RGBA = {
      static_cast<int>(MyImGui::ZombieVerticalLineColor.x * 255),
      static_cast<int>(MyImGui::ZombieVerticalLineColor.y * 255),
      static_cast<int>(MyImGui::ZombieVerticalLineColor.z * 255),
      static_cast<int>(MyImGui::ZombieVerticalLineColor.w * 255)};

  return 1;
}

/*
    brief: Draw color picker window
*/
bool MyImGui::DrawColorPickerWindow() {
  ImGui::SetNextWindowPos({50, 400}, ImGuiCond_FirstUseEver);

  ImGui::Begin("Color Picker", 0, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::ColorEdit4("Menu Color", (float *)&MyImGui::MenuThemeColor,
                    ImGuiColorEditFlags_DisplayRGB);
  ImGui::ColorEdit4("Tracer Color", (float *)&MyImGui::ZombieTracerColor,
                    ImGuiColorEditFlags_DisplayRGB);
  ImGui::ColorEdit4("Vertical Line Color",
                    (float *)&MyImGui::ZombieVerticalLineColor,
                    ImGuiColorEditFlags_DisplayRGB);

  ImGui::End();
  return 1;
}

/*
    brief: Draw combat window
*/
bool MyImGui::DrawCombatWindow() {
  ImGui::SetNextWindowPos({400, 100}, ImGuiCond_FirstUseEver);

  ImGui::Begin("Combat", 0, ImGuiWindowFlags_AlwaysAutoResize);

  if (ImGui::Button("Toggle Infinite Ammo")) {
    Hack::ToggleInfiniteAmmo(!Config::InfiniteAmmo);
  }

  ImGui::Checkbox("Silent Reload", &Config::SilentReload);

  ImGui::End();
  return 1;
}

/*
    brief: Draw visual window
*/
bool MyImGui::DrawVisualWindow() {
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
  return 1;
}

/*
    brief: My debug ImGui window with WIP stuff
*/
bool MyImGui::DrawDebugWindow() {


 ImGui::Begin("Debug Window");

  if (GetAsyncKeyState(VK_DELETE) & 1) {
    Hack::ShootGun();
  }
  ImGui::End();
  return 1;
}

/*
    brief: Shows all of my main ImGui windows
*/
bool MyImGui::ShowMyWindows() {

  MyImGui::DrawColorPickerWindow();

  MyImGui::DrawCombatWindow();

  MyImGui::DrawVisualWindow();

  MyImGui::DrawDebugWindow();

  return true;
}

/*
    brief: Run on the initial setup of ImGui. Does DirectX stuff and sets
   initial colors
*/
ImVec4 MyImGui::MenuThemeColor;
ImVec4 MyImGui::ZombieTracerColor;
ImVec4 MyImGui::ZombieVerticalLineColor;
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

  MyImGui::MenuThemeColor =
      ImVec4(186.0f / 255.0f, 8.0f / 255.0f, 189.0f / 255.0f, 255.0f / 255.0f);

  MyImGui::ZombieTracerColor = ImVec4(1, 1, 1, 1);

  MyImGui::ZombieVerticalLineColor = ImVec4(155.0f / 255.0f, 155.0f / 255.0f,
                                            155.0f / 255.0f, 255.0f / 255.0f);

  return true;
}