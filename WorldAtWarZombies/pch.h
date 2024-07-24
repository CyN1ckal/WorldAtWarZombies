// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <tchar.h>

#include <iostream>
#include <string>
#include <format>
#include <vector>

#include <MinHook.h>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Config.h"

#include "MyImGui.h"

#include "Offsets.h"

#include "Vector.h"

#include "Reclass.h"

#include "StartingThread.h"

#include "Hook.h"

#include "Draw.h"

#include "Hack.h"

#include "Memory.h"

#include "Debug.h"

#endif //PCH_H
