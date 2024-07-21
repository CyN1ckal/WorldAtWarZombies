#pragma once

class MyImGui {
 public:
  static bool Initialized;
  static bool ShowMyWindow();
  static bool Initialize(LPDIRECT3DDEVICE9 pD3DDevice);
};