#include "pch.h"

std::vector<CEntity> Hack::AliveZombieVector;

/*
    brief: Turns on the infinite ammo hack by patching the function which
   decreases the ammo amount
*/
bool Hack::ToggleInfiniteAmmo(bool b) {
  uintptr_t DecrementAmmoFunction =
      WaW_BaseAddress + Offsets::DecrementAmmoOffset;
  DWORD protection;

  VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                   PAGE_READWRITE, &protection);

  if (b == true) {

    memset((void *)DecrementAmmoFunction, 0x90, 7);

    Config::InfiniteAmmo = true;

  } else {

    BYTE OriginalBytes[] = {0x89, 0x84, 0x8F, 0xFC, 0x05, 0x00, 0x00};

    memcpy((void *)DecrementAmmoFunction, &OriginalBytes, 7);

    Config::InfiniteAmmo = false;
  }

  VirtualProtectEx(GetCurrentProcess(), (void *)DecrementAmmoFunction, 7,
                   protection, nullptr);

  return 1;
}

/*
    brief: This was an initial test of how writing to view angles in this game
   works
*/
bool Hack::ResetViewAngles() {
  WritableAngles *Angles =
      (WritableAngles *)(WaW_BaseAddress + Offsets::WritableAngleOffset);

  CenterDifference *Difference =
      (CenterDifference *)(WaW_BaseAddress + Offsets::CenterDifferenceOffset);

  Angles->Pitch -= Difference->Pitch;
  Angles->Yaw -= Difference->Yaw;

  return 1;
}

/*
    brief: Aimbot v1; aims at center mass rather than the head.
    Loops through the ent state array rather than the CEnt array, so also slow.
*/
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

/*
    brief: Aims at the closest alive zombie (head) in the AliveZombieVector
*/
bool Hack::AimAtClosestZombieHead() {

  float ClosestZombieDistance = 100000.0f;
  int ClosestZombieNumber = -1;

  for (int i = 0; i < Hack::AliveZombieVector.size(); i++) {
    float CurrentZombieDistance =
        VecDistance(Hack::LocalPlayerCamera->Origin,
                    Hack::AliveZombieVector[i].HeadPosition);

    if (CurrentZombieDistance < ClosestZombieDistance) {
      ClosestZombieDistance = CurrentZombieDistance;
      ClosestZombieNumber = i;
    }
  }
  if (ClosestZombieNumber == -1)
    return 0;

  Vector3 TargetPos = Hack::AliveZombieVector[ClosestZombieNumber].HeadPosition;

  Hack::AimAtPosition(TargetPos);

  return 1;
}

/*
    brief: Updates the ZombieVector in the Hack class
*/
bool Hack::FillZombieVector() {
  Hack::AliveZombieVector.clear();

  CEntBaseArray_New *CEntArray = *(
      CEntBaseArray_New **)(Hack::WaW_BaseAddress + Offsets::CEntArraypOffset);

  for (int i = 0; i < MAX_CENTS; i++) {

    if (CEntArray->CEntArray[i].NextEntState == nullptr)
      continue;
    if (Hack::EntityStateArray
            ->EntityStateArray[CEntArray->CEntArray[i].EntStateArrayNumber]
            .CurrentHealth < 1)
      continue;

    Hack::AliveZombieVector.push_back(CEntArray->CEntArray[i]);
  }

  return 1;
}

/*
    brief: Automatically reloads whatever weapon you are holding. I had to do
   in-line ASM because regardless of whatever calling convention I used I was
   getting a crash...
   I am probably just being stupid but this works and I learned how C++ asm
   works... Win for me
*/
bool Hack::SilentReload() {
  DWORD LocalPlayer_4 = 0x018ED068;
  __asm {
          mov esi, LocalPlayer_4
  }
  Hack::Reload_FunctionCall();
  return 1;
}

/*
    brief: Aim the local player at 3d position
*/
bool Hack::AimAtPosition(Vector3 TargetPos) {
  Vector3 LocalPos = Hack::LocalPlayerCamera->Origin;

  Vector3 PositionDelta = {TargetPos.x - LocalPos.x, TargetPos.y - LocalPos.y,
                           TargetPos.z - LocalPos.z};

  /*
    First, handle angles which are positive Y
  */
  if (PositionDelta.y > 0) {
    // Yaw Calculation
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / PI);

    // Setting Yaw
    LocalPlayerWritableAngles->Yaw =
        LocalPlayerWritableAngles->Yaw -
        Hack::LocalPlayerCamera->CenterDifference_Yaw + 90.0f - Degrees;

    // Pitch Calculation
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));
    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / PI);

    // Setting Pitch
    LocalPlayerWritableAngles->Pitch =
        LocalPlayerWritableAngles->Pitch -
        Hack::LocalPlayerCamera->CenterDifference_Pitch - Degrees;

  }
  /*
    Next, handle angles which are negative Y
  */
  else if (PositionDelta.y < 0) {
    // Yaw Calculations
    float OppAdj = PositionDelta.x / PositionDelta.y;
    float Degrees = atan(OppAdj) * (180 / PI);

    // Setting Yaw
    LocalPlayerWritableAngles->Yaw =
        LocalPlayerWritableAngles->Yaw -
        Hack::LocalPlayerCamera->CenterDifference_Yaw - 90.0f - Degrees;

    // Pitch Calculations
    float XY_Distance = sqrt((PositionDelta.x * PositionDelta.x) +
                             (PositionDelta.y * PositionDelta.y));
    OppAdj = PositionDelta.z / XY_Distance;
    Degrees = atan(OppAdj) * (180 / PI);

    // Setting Pitch
    LocalPlayerWritableAngles->Pitch =
        LocalPlayerWritableAngles->Pitch -
        Hack::LocalPlayerCamera->CenterDifference_Pitch - Degrees;
  }
  return 1;
}

/*
    brief: calls the "Shoot" function; but the bullets that get shot do no
   damage... Useless but kinda funny
*/
bool Hack::ShootGun() {
  DWORD LocalPlayer_4 = 0x018ED068;

  ShootWrapper_Template ShootWrapper = (ShootWrapper_Template)0x00420A60;

  ShootWrapper(LocalPlayer_4, 2);

  return 1;
}