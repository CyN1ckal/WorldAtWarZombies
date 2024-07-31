#pragma once
typedef int(APIENTRY *PrintToConsole_Template)(int a1, int a2, ...);
typedef void(APIENTRY *PrintErrorToConsole_Template)(int a1, int a2, ...);
typedef void(__cdecl *PrintRawToConsole_Template)(int a1, const char *a2,
                                                  int a3);
typedef int(__stdcall *Reload_Template)();

typedef char(__cdecl *Shoot_Template)(int LocalPlayer, int One,
                                      int WeaponPtrArrayIndex);

class Hack {
public:
  static inline uintptr_t WaW_BaseAddress = (uintptr_t)GetModuleHandle(NULL);
  static std::vector<CEntity> AliveZombieVector;

  // Function Call Declarations
  static inline PrintToConsole_Template PrintToConsole =
      reinterpret_cast<PrintToConsole_Template>(Offsets::PrintToConsoleOffset);

  static inline PrintErrorToConsole_Template PrintErrorToConsole =
      reinterpret_cast<PrintErrorToConsole_Template>(
          Offsets::PrintErrorToConsoleOffset);

  static inline PrintRawToConsole_Template PrintRawToConsole =
      reinterpret_cast<PrintRawToConsole_Template>(
          Offsets::PrintRawToConsoleOffset);

  static inline Reload_Template Reload_FunctionCall =
      reinterpret_cast<Reload_Template>(WaW_BaseAddress +
                                        Offsets::ReloadMaybeOffset);

  static inline Shoot_Template Shoot_FunctionCall =
      reinterpret_cast<Shoot_Template>(Hack::WaW_BaseAddress +
                                       Offsets::ShootOffset);

  // Static address declarations
  static inline refdef_t *RefDef =
      reinterpret_cast<refdef_t *>(WaW_BaseAddress + Offsets::RefDefOffset);

  static inline LocalPlayer *Local_Player =
      (LocalPlayer *)(WaW_BaseAddress + Offsets::LocalPlayerOffset);
  static inline auto *pViewMatrix = (float *)(Offsets::ViewMatrixOffset);

  static inline Camera_Class *LocalPlayerCamera =
      *(Camera_Class **)(WaW_BaseAddress + Offsets::CameraClasspOffset);

  static inline EntityStateArray_New *EntityStateArray =
      *(EntityStateArray_New **)(Hack::WaW_BaseAddress +
                                 Offsets::EntityStateArrayppOffset);

  static inline WritableAngles *LocalPlayerWritableAngles =
      (WritableAngles *)(Hack::WaW_BaseAddress + Offsets::WritableAngleOffset);

  // Hack class member functions
  static bool ToggleInfiniteAmmo(bool b);
  static bool ResetViewAngles();
  static bool AimAtClosestZombie();
  static bool AimAtClosestZombieHead();
  static bool FillZombieVector();
  static bool SilentReload();
  static bool AimAtPosition(Vector3 Target);
  static bool ShootGun();

private:
};