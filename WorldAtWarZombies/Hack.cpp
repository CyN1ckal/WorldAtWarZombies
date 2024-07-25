#include "pch.h"

/*
        brief: Gets the number of zombies remaining by looping through the
   entity list looking for the correct Type
*/
int Hack::GetNumZombies() {
  CEntBaseArray_New *CEntArray =
      *(CEntBaseArray_New **)(WaW_BaseAddress + Offsets::CEntArraypOffset);

  EntityStateArray_New *EntStateArray =
      *(EntityStateArray_New **)(WaW_BaseAddress +
                                 Offsets::EntityStateArrayppOffset);

  int AliveZombies = 0;

  for (int i = 0; i < MAX_CENTS; i++) {
    if (CEntArray->CEntArray[i].NextEntState == nullptr)
      continue;

    if (EntStateArray
            ->EntityStateArray[CEntArray->CEntArray[i].EntStateArrayNumber]
            .CurrentHealth < 1)
      continue;

    AliveZombies++;
  }

  return AliveZombies;
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

  for (int i = 0; i < MAX_ENTSTATES; i++) {
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

  if ((PositionDelta.x > 0 && PositionDelta.y > 0) ||
      ((PositionDelta.x < 0 && PositionDelta.y > 0))) {
    // Yaw Calculations
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / PI);

    // Setting Yaw
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw + 90.0f - Degrees;

    // Pitch Adjustment
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));

    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / PI);
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;

  } else if ((PositionDelta.x < 0 && PositionDelta.y < 0) ||
             (PositionDelta.x > 0 && PositionDelta.y < 0)) {
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / PI);
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw - 90.0f - Degrees;

    // Pitch Adjustment
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));

    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / PI);
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;
  }

  return 1;
}

bool Hack::AimAtClosestZombieHead() {

  EntityStateArray_New *EntityStateArray =
      *(EntityStateArray_New **)(Hack::WaW_BaseAddress +
                                 Offsets::EntityStateArrayppOffset);

  CEntBaseArray_New *CEntArray = *(
      CEntBaseArray_New **)(Hack::WaW_BaseAddress + Offsets::CEntArraypOffset);

  float ClosestZombieDistance = 100000.0f;
  int ClosestZombieNumber = -1;
  int ClosestZombieEntStateArray = -1;

  for (int i = 0; i < MAX_CENTS; i++) {

    if (CEntArray->CEntArray[i].NextEntState == nullptr)
      continue;
    if (EntityStateArray
            ->EntityStateArray[CEntArray->CEntArray[i].EntStateArrayNumber]
            .CurrentHealth < 1)
      continue;

    float CurrentZombieDistance = VecDistance(
        Local_Player->position, CEntArray->CEntArray[i].HeadPosition);

    if (CurrentZombieDistance < ClosestZombieDistance) {
      ClosestZombieDistance = CurrentZombieDistance;
      ClosestZombieNumber = i;
    }
  }

  if (ClosestZombieNumber == -1)
    return 0;

  WritableAngles *Angles =
      (WritableAngles *)(WaW_BaseAddress + Offsets::WritableAngleOffset);

  Camera_Class *Cam =
      *(Camera_Class **)(WaW_BaseAddress + Offsets::CameraClasspOffset);

  Vector3 TargetPos = CEntArray->CEntArray[ClosestZombieNumber].HeadPosition;

  Vector3 LocalPos = Cam->Origin;

  Vector3 PositionDelta = {TargetPos.x - LocalPos.x, TargetPos.y - LocalPos.y,
                           TargetPos.z - LocalPos.z};

  if ((PositionDelta.x > 0 && PositionDelta.y > 0) ||
      ((PositionDelta.x < 0 && PositionDelta.y > 0))) {
    // Yaw Calculation
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / 3.141592653);

    // Setting Yaw
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw + 90.0f - Degrees;

    // Pitch Calculation
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));
    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / 3.141592653);

    // Setting Pitch
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;

  } else if ((PositionDelta.x < 0 && PositionDelta.y < 0) ||
             (PositionDelta.x > 0 && PositionDelta.y < 0)) {
    // Yaw Calculations
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / 3.141592653);

    // Setting Yaw
    Angles->Yaw = Angles->Yaw - Cam->CenterDifference_Yaw - 90.0f - Degrees;

    // Pitch Calculations
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));
    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / 3.141592653);

    // Setting Pitch
    Angles->Pitch = Angles->Pitch - Cam->CenterDifference_Pitch - Degrees;
  }

  return 1;
}