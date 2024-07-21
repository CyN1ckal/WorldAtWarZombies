#include "pch.h"

bool Config::Initialize()
{
	TracerLines = true;
	MasterImgui = true;
	LocalPlayerHealthBar = true;
	ZombieCount = true;

	return true;
}