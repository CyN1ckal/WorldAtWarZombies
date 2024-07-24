#include "pch.h"

bool Config::Initialize() {
  TracerLines = true;
  MasterImgui = true;
  LocalPlayerHealthBar = true;
  ZombieCount = true;
  TypeTracers = false;
  TypeNumber = 6;
  InfiniteAmmo = false;
  DebugVisuals = false;
  AimbotToggle = false;

  return true;
}