#pragma once

/*
        brief: Contains all the offsets required
*/
namespace Offsets {
static uintptr_t PrintToConsoleOffset = 0x59A2C0;
static uintptr_t PrintErrorToConsoleOffset = 0x59A380;
static uintptr_t PrintRawToConsoleOffset = 0x59A170;
static uintptr_t RefDefOffset = 0x3120338;
static uintptr_t LocalPlayerOffset = 0x136c6f0;
static uintptr_t ViewMatrixOffset = 0x008E870C;
static uintptr_t EntityStateArrayOffset = 0xBE1C0;
static uintptr_t WndProcOffset = 0x206BE0;
static uintptr_t DecrementAmmoOffset = 0x1E619;
}  // namespace Offsets