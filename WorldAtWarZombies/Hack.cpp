#include "pch.h"


void Hack::PrintAliveEnts()
{
	EntityListStruct* EntListStruct = (EntityListStruct*)((uintptr_t)GetModuleHandle(NULL) + 0x14e74d0);

	int Alive = 0;
	int Dead = 0;
	for (int i = 0; i < 32; i++)
	{
		if (EntListStruct->PtrToEntArray->EntArray[i].CurrentHealth > 0) Alive++;
	}

	std::cout << Alive << std::endl;
}

