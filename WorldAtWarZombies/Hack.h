#pragma once
typedef int(APIENTRY* PrintToConsole_Template)(int a1, int a2, ...);
typedef void(APIENTRY* PrintErrorToConsole_Template)(int a1, int a2, ...);
typedef void(__cdecl* PrintRawToConsole_Template)(int a1,
                                                  const char* a2,
                                                  int a3);

class Hack {
 public:
  // Function Call Declarations
  static inline PrintToConsole_Template PrintToConsole =
      reinterpret_cast<PrintToConsole_Template>(Offsets::PrintToConsoleOffset);
  static inline PrintErrorToConsole_Template PrintErrorToConsole =
      reinterpret_cast<PrintErrorToConsole_Template>(
          Offsets::PrintErrorToConsoleOffset);
  static inline PrintRawToConsole_Template PrintRawToConsole =
      reinterpret_cast<PrintRawToConsole_Template>(
          Offsets::PrintRawToConsoleOffset);

  // Static address declarations
  static inline uintptr_t WaW_BaseAddress = (uintptr_t)GetModuleHandle(NULL);
  static inline refdef_t* RefDef =
      reinterpret_cast<refdef_t*>(WaW_BaseAddress + Offsets::RefDefOffset);
  static inline LocalPlayer* Local_Player =
      (LocalPlayer*)(WaW_BaseAddress + Offsets::LocalPlayerOffset);
  static inline auto* pViewMatrix = (float*)(Offsets::ViewMatrixOffset);

  // Hack class member functions
  static int GetNumZombies();
  static bool ToggleInfiniteAmmo(bool b);

 private:
};