#include "pch.h"

/*
        brief: Gets the number of zombies remaining by looping through the
   entity list looking for the correct Type
*/
int Hack::GetNumZombies() {
  EntityStateArray_New *EntityStateArray =
      *(EntityStateArray_New **)(Hack::WaW_BaseAddress +
                                 Offsets::EntityStateArrayOffset);

  int ZombieCount = 0;

  for (int i = 0; i < 1024; i++) {
    if (EntityStateArray->EntityStateArray[i].eType == EntityType::Zombie &&
        EntityStateArray->EntityStateArray[i].CurrentHealth > 0) {
      ZombieCount++;
    }
  }

  return ZombieCount;
}

/*
    brief: Turns on the infinite ammo hack by patching the function which
   decreases the ammo amount
*/
bool Hack::ToggleInfiniteAmmo(bool b) {
  uintptr_t DecrementAmmoFunction =
      WaW_BaseAddress + Offsets::DecrementAmmoOffset;

  if (b == true) {
    DWORD protection;
    VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                     PAGE_READWRITE, &protection);
    memset((void *)DecrementAmmoFunction, 0x90, 7);
    VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                     protection, nullptr);
    Config::InfiniteAmmo = true;
    return 1;

  } else {
    DWORD protection;
    BYTE OriginalBytes[] = {0x89, 0x84, 0x8F, 0xFC, 0x05, 0x00, 0x00};
    VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                     PAGE_READWRITE, &protection);
    memcpy((void *)DecrementAmmoFunction, &OriginalBytes, 7);
    VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                     protection, nullptr);
    Config::InfiniteAmmo = false;
    return 0;
  }
}

bool Hack::ResetViewAngles() {
  WritableAngles *Angles =
      (WritableAngles *)(WaW_BaseAddress + Offsets::WritableAngleOffset);

  CenterDifference *Difference =
      (CenterDifference *)(WaW_BaseAddress + Offsets::CenterDifferenceOffset);

  Angles->Pitch -= Difference->Pitch;
  Angles->Yaw -= Difference->Yaw;
  return 1;
}

bool Hack::AimAtClosestZombie() {
  EntityStateArray_New *EntityStateArray =
      *(EntityStateArray_New **)(Hack::WaW_BaseAddress +
                                 Offsets::EntityStateArrayppOffset);

  float ClosestZombieDistance = 100000.0f;
  int ClosestZombieNumber = 0;

  for (int i = 0; i < 1024; i++) {
    if (EntityStateArray->EntityStateArray[i].eType == EntityType::Zombie &&
        EntityStateArray->EntityStateArray[i].CurrentHealth > 0) {
      float CurrentZombieDistance =
          VecDistance(Local_Player->position,
                      EntityStateArray->EntityStateArray[i].position);
      if (CurrentZombieDistance < ClosestZombieDistance) {
        ClosestZombieDistance = CurrentZombieDistance;
        ClosestZombieNumber = i;
      }
    }
  }

  if (ClosestZombieNumber == 0)
    return 0;

  // std::cout << std::format("Closest Zombie: {}, {}", ClosestZombieDistance,
  //                          ClosestZombieNumber)
  //           << std::endl;

  WritableAngles *Angles =
      (WritableAngles *)(WaW_BaseAddress + Offsets::WritableAngleOffset);

  Camera_Class *Cam =
      *(Camera_Class **)(WaW_BaseAddress + Offsets::CameraClasspOffset);

  Vector3 TargetPos =
      EntityStateArray->EntityStateArray[ClosestZombieNumber].position;

  TargetPos.z += 45.0f;

  Vector3 LocalPos = Cam->Origin;

  Vector3 PositionDelta = {TargetPos.x - LocalPos.x, TargetPos.y - LocalPos.y,
                           TargetPos.z - LocalPos.z};

  // printf("%f,%f,%f\n", PositionDelta.x, PositionDelta.y, PositionDelta.z);

  if ((PositionDelta.x > 0 && PositionDelta.y > 0) ||
      ((PositionDelta.x < 0 && PositionDelta.y > 0))) {
    // Yaw Adjustment
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / 3.141592653);
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw + 90.0f - Degrees;

    // Pitch Adjustment
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                          (PositionDelta.y * PositionDelta.y));

    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / 3.141592653);
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;

  } else if ((PositionDelta.x < 0 && PositionDelta.y < 0) ||
             (PositionDelta.x > 0 && PositionDelta.y < 0)) {
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / 3.141592653);
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw - 90.0f - Degrees;

    // Pitch Adjustment
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));

    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / 3.141592653);
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;
  }

  return 1;
}