#pragma once

struct RGBA_Struct {
  int r, g, b, a;
};

class MyImGui {
public:
  static bool Initialized;
  static bool ShowMyWindows();
  static bool DrawColorPickerWindow();
  static bool DrawVisualWindow();
  static bool DrawCombatWindow();
  static bool DrawDebugWindow();
  static bool Initialize(LPDIRECT3DDEVICE9 pD3DDevice);

  static bool UpdateColors();

  static ImVec4 MenuThemeColor;
  static RGBA_Struct MenuThemeColor_RGBA;

  static ImVec4 ZombieTracerColor;
  static RGBA_Struct ZombieTracerColor_RGBA;

  static ImVec4 ZombieVerticalLineColor;
  static RGBA_Struct ZombieVerticalLineColor_RGBA;
};