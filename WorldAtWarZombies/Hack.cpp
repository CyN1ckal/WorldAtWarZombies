#include "pch.h"

/*
	brief: Gets the number of zombies remaining by looping through the entity list looking for the correct Type
*/
int Hack::GetNumZombies()
{
	EntityStateArray_New* EntityStateArray = *(EntityStateArray_New**)(Hack::WaW_BaseAddress + 0xbe1c0);

	int ZombieCount = 0;

	for (int i = 0; i < 1024; i++)
	{
		if (EntityStateArray->EntityStateArray[i].eType == 16)
		{
			ZombieCount++;
			//printf("Zombie ID: %d\n", i);
		}
	}

	return ZombieCount;
}