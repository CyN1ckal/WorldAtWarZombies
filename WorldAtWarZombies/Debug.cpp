#include "pch.h"

bool Debug::Initialized;

bool Debug::Initialize() {
  Debug::Initialized = true;
  return 1;
}

bool Debug::PrintFacingDirection() {
  RECT CoordinateRect = {0, 50, 300, 74};
  if (Hack::Local_Player->viewAngles.y < 45.0f &&
      Hack::Local_Player->viewAngles.y > -45.0f) {
    std::string DirectionText =
        std::format("Facing +X (East) {}", Hack::Local_Player->viewAngles.y);
    Draw::pFont[2]->DrawTextA(NULL, DirectionText.c_str(), -1, &CoordinateRect,
                              DT_LEFT | DT_TOP,
                              D3DCOLOR_ARGB(255, 255, 255, 255));
  } else if (Hack::Local_Player->viewAngles.y <= -45.0f &&
             Hack::Local_Player->viewAngles.y >= -135.0f) {
    std::string DirectionText =
        std::format("Facing -Y (South) {}", Hack::Local_Player->viewAngles.y);
    Draw::pFont[2]->DrawTextA(NULL, DirectionText.c_str(), -1, &CoordinateRect,
                              DT_LEFT | DT_TOP,
                              D3DCOLOR_ARGB(255, 255, 255, 255));
  } else if (Hack::Local_Player->viewAngles.y <= 135.0f &&
             Hack::Local_Player->viewAngles.y >= 45.0f) {
    std::string DirectionText =
        std::format("Facing +Y (North) {}", Hack::Local_Player->viewAngles.y);
    Draw::pFont[2]->DrawTextA(NULL, DirectionText.c_str(), -1, &CoordinateRect,
                              DT_LEFT | DT_TOP,
                              D3DCOLOR_ARGB(255, 255, 255, 255));
  } else if ((Hack::Local_Player->viewAngles.y < -135.0f &&
              Hack::Local_Player->viewAngles.y > -180.0f) ||
             (Hack::Local_Player->viewAngles.y < 180.0f &&
              Hack::Local_Player->viewAngles.y > 135.0f)) {
    std::string DirectionText =
        std::format("Facing -X (West) {}", Hack::Local_Player->viewAngles.y);
    Draw::pFont[2]->DrawTextA(NULL, DirectionText.c_str(), -1, &CoordinateRect,
                              DT_LEFT | DT_TOP,
                              D3DCOLOR_ARGB(255, 255, 255, 255));
  }

  return 1;
}

bool Debug::PrintPlayerCoordinates() {
  std::string PlayerCoordinateString = std::format(
      "{},{},{}", Hack::Local_Player->position.x,
      Hack::Local_Player->position.y, Hack::Local_Player->position.z);

  RECT PlayerCoordinateRect = {0, 75, 300, 99};

  Draw::pFont[2]->DrawTextA(NULL, PlayerCoordinateString.c_str(), -1,
                            &PlayerCoordinateRect, DT_LEFT | DT_TOP,
                            D3DCOLOR_ARGB(255, 255, 255, 255));
  return 1;
}