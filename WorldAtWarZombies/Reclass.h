#pragma once
// Created with ReClass.NET 1.2 by KN4CK3R

struct Vector3
{
	float x, y, z;
};

struct Vector2
{
	float x, y;
};
// Created with ReClass.NET 1.2 by KN4CK3R

enum class EntityType : int32_t
{
	DeadZombie = 18,
	Empty = 0,
	Prop = 6,
	Zombie = 16
};

class NumZombies
{
public:
	int32_t NumZombies; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class EntityStateArrayBase
{
public:
	class EntityState(*EntityStateArrayPtr)[1000]; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class EntityState
{
public:
	int32_t number; //0x0000
	EntityType eType; //0x0004
	int32_t eFlags; //0x0008
	int32_t eFlags2; //0x000C
	char pad_0010[8]; //0x0010
	Vector3 position; //0x0018
	char pad_0024[152]; //0x0024
	int32_t TimesHitPlayer; //0x00BC
	char pad_00C0[136]; //0x00C0
	Vector3 position2; //0x0148
	Vector3 position3; //0x0154
	Vector3 position4; //0x0160
	Vector3 viewAngles; //0x016C
	char pad_0178[76]; //0x0178
	int32_t time; //0x01C4
	char pad_01C8[276]; //0x01C8
	int32_t unknown1; //0x02DC
	char pad_02E0[152]; //0x02E0
}; //Size: 0x0378

class LocalPlayer
{
public:
	char pad_0000[24]; //0x0000
	Vector3 position; //0x0018
	char pad_0024[24]; //0x0024
	Vector3 viewAngles; //0x003C
	char pad_0048[168]; //0x0048
	int32_t PlayerState; //0x00F0
	char pad_00F4[200]; //0x00F4
	int32_t Time; //0x01BC
	char pad_01C0[8]; //0x01C0
	int32_t CurrentHealth; //0x01C8
	int32_t MaxHealth; //0x01CC
	char pad_01D0[1664]; //0x01D0
}; //Size: 0x0850