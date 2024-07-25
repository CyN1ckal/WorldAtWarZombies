// Created with ReClass.NET 1.2 by KN4CK3R

enum class EntityType : int32_t {
  DeadZombie = 18,
  Empty = 0,
  Prop = 6,
  Zombie = 16
};

class EntityState {
public:
  int32_t number;         // 0x0000
  EntityType eType;       // 0x0004
  int32_t eFlags;         // 0x0008
  int32_t eFlags2;        // 0x000C
  char pad_0010[8];       // 0x0010
  Vector3 position;       // 0x0018
  char pad_0024[132];     // 0x0024
  int32_t unknown2;       // 0x00A8 66 dying anim?
  char pad_00AC[16];      // 0x00AC
  int32_t TimesHitPlayer; // 0x00BC
  char pad_00C0[136];     // 0x00C0
  Vector3 position2;      // 0x0148
  Vector3 position3;      // 0x0154
  Vector3 position4;      // 0x0160
  Vector3 viewAngles;     // 0x016C
  char pad_0178[76];      // 0x0178
  int32_t time;           // 0x01C4
  int32_t CurrentHealth;  // 0x01C8
  int32_t MaxHealth;      // 0x01CC
  char pad_01D0[268];     // 0x01D0
  int32_t unknown1;       // 0x02DC
  char pad_02E0[152];     // 0x02E0
};                        // Size: 0x0378

class LocalPlayer {
public:
  char pad_0000[8];      // 0x0000
  int32_t Flags;         // 0x0008 8 Dead/Dying, 2 EndGame Camera, 0 Playing
  char pad_000C[12];     // 0x000C
  Vector3 position;      // 0x0018
  char pad_0024[24];     // 0x0024
  Vector3 viewAngles;    // 0x003C
  char pad_0048[168];    // 0x0048
  int32_t PlayerState;   // 0x00F0
  char pad_00F4[200];    // 0x00F4
  int32_t Time;          // 0x01BC
  char pad_01C0[8];      // 0x01C0
  int32_t CurrentHealth; // 0x01C8
  int32_t MaxHealth;     // 0x01CC
  char pad_01D0[1664];   // 0x01D0
};                       // Size: 0x0850

class EntityStateArray_New {
public:
  class EntityState EntityStateArray[1024]; // 0x0000
};                                          // Size: 0xDE000

class refdef_t_2 {
public:
  int32_t Width;               // 0x0000
  int32_t Height;              // 0x0004
  int32_t Width2;              // 0x0008
  int32_t Height2;             // 0x000C
  char pad_0010[12];           // 0x0010
  float AspectRatio;           // 0x001C
  char pad_0020[124];          // 0x0020
  Vector3 ViewMatrix_Maybe;    // 0x009C
  Vector3 ProjectMatrix_Maybe; // 0x00A8
  char pad_00B4[1180];         // 0x00B4
};                             // Size: 0x0550

class refdef_t {
public:
  char pad_0000[8];  // 0x0000
  int32_t Width;     // 0x0008
  int32_t Height;    // 0x000C
  float Fov_x_Maybe; // 0x0010
  float Fov_y_Maybe; // 0x0014
  float cg_fov;      // 0x0018
  Vector3 viewOrg;   // 0x001C
  char pad_0028[4];  // 0x0028
  Vector3 viewaxis1; // 0x002C
  Vector3 viewaxis2; // 0x0038
  Vector3 viewaxis3; // 0x0044
  Vector3 viewOrg2;  // 0x0050
  int32_t time;      // 0x005C
};                   // Size: 0x0060

class WritableAngles {
public:
  float Pitch;       // 0x0000
  float Yaw;         // 0x0004
  float Roll;        // 0x0008
  char pad_000C[52]; // 0x000C
};                   // Size: 0x0040

class CenterDifference {
public:
  float Pitch;       // 0x0000
  float Yaw;         // 0x0004
  char pad_0008[56]; // 0x0008
};                   // Size: 0x0040

class Camera_Class {
public:
  char pad_0000[36];            // 0x0000
  Vector3 Origin;               // 0x0024
  float CenterDifference_Pitch; // 0x0030
  float CenterDifference_Yaw;   // 0x0034
  char pad_0038[16];            // 0x0038
};                              // Size: 0x0048

class CEntity {
public:
  class EntityState *CurrentEntState; // 0x0000
  class EntityState *NextEntState;    // 0x0004
  char pad_0008[4];                   // 0x0008
  int32_t CEntArrayNumber;            // 0x000C
  int32_t EntStateArrayNumber;        // 0x0010
  int32_t AliveFlag;                  // 0x0014
  int32_t UnknownFlags1;              // 0x0018
  int32_t UnknownFlags2;              // 0x001C
  int32_t UnknownFlags3;              // 0x0020
  char pad_0024[3272];                // 0x0024
  Vector3 HeadPosition;               // 0x0CEC
  char pad_0CF8[9408];                // 0x0CF8
};                                    // Size: 0x31B8

class CEntArrayBase {
public:
  class CEntity (*N00000ED5)[25]; // 0x0000
};                                // Size: 0x0004

class CEntBaseArray_New {
public:
  class CEntity CEntArray[25]; // 0x0000
};                             // Size: 0x4DAF8

class N00001511 {
public:
  char pad_0000[4]; // 0x0000
};                  // Size: 0x0004