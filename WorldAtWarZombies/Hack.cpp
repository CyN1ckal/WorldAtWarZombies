#include "pch.h"

/*
        brief: Gets the number of zombies remaining by looping through the
   entity list looking for the correct Type
*/
int Hack::GetNumZombies() {
  EntityStateArray_New* EntityStateArray =
      *(EntityStateArray_New**)(Hack::WaW_BaseAddress +
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
    brief: Turns on the infinite ammo hack by patching the function which decreases the ammo amount
*/
bool Hack::ToggleInfiniteAmmo(bool b) {
  uintptr_t DecrementAmmoFunction =
      WaW_BaseAddress + Offsets::DecrementAmmoOffset;

  if (b == true) {
    DWORD protection;
    VirtualProtectEx(GetCurrentProcess(), (void*)DecrementAmmoFunction, 7,
                     PAGE_READWRITE, &protection);
   memset((void*)DecrementAmmoFunction, 0x90, 7);
    VirtualProtectEx(GetCurrentProcess(), (void*)DecrementAmmoFunction, 7,
                     protection,nullptr);
    Config::InfiniteAmmo = true;
    return 1;

  } else {
    DWORD protection;
    BYTE OriginalBytes[] = {0x89, 0x84, 0x8F, 0xFC, 0x05, 0x00, 0x00};
    VirtualProtectEx(GetCurrentProcess(), (void*)DecrementAmmoFunction, 7,
                     PAGE_READWRITE, &protection);
    memcpy((void*)DecrementAmmoFunction, &OriginalBytes, 7);
    VirtualProtectEx(GetCurrentProcess(), (void*)DecrementAmmoFunction, 7,
                     protection, nullptr);
    Config::InfiniteAmmo = false;
    return 0;
  }
}