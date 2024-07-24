#pragma once

class Debug
{
 public:
  static bool Initialize();
  static bool Initialized;

  static bool PrintFacingDirection();
  static bool PrintPlayerCoordinates();

 private:
};