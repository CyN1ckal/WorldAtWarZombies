#pragma once

class Config {
 public:
  static bool TracerLines;
  static bool MasterImgui;
  static bool LocalPlayerHealthBar;
  static bool ZombieCount;
  static bool TypeTracers;
  static int TypeNumber;
  static bool InfiniteAmmo;
  static bool DebugVisuals;
  static bool VerticalLineESP;

  static bool Initialize();
};
