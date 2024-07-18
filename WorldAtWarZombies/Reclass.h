#pragma once
// Created with ReClass.NET 1.2 by KN4CK3R

struct Vector3
{
	float x, y, z;
};
// Created with ReClass.NET 1.2 by KN4CK3R

class entity
{
public:
	int32_t EntID; //0x0000
	char pad_0004[20]; //0x0004
	Vector3 Position; //0x0018
	char pad_0024[24]; //0x0024
	Vector3 ViewAngles; //0x003C
	char pad_0048[4]; //0x0048
	void* Bot_vTable; //0x004C
	char pad_0050[4]; //0x0050
	int32_t Bot_ID; //0x0054
	char pad_0058[368]; //0x0058
	int32_t CurrentHealth; //0x01C8
	int32_t MaxHealth; //0x01CC
	char pad_01D0[424]; //0x01D0
}; //Size: 0x0378

class EntityListStruct
{
public:
	class N00001F69* PtrToEntArray; //0x0000
	char pad_0004[28540]; //0x0004
}; //Size: 0x6F80

class NumZombies
{
public:
	int32_t NumZombies; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class EntArray : public entity
{
public:
	char pad_0378[256]; //0x0378
}; //Size: 0x0478

class N00001F69
{
public:
	class entity EntArray[32]; //0x0000
}; //Size: 0x6F00