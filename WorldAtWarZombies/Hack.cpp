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