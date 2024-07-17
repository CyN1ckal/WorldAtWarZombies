// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <iostream>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")



#include <MinHook.h>

#include "StartingThread.h"

#include "Hook.h"

#include "Draw.h"

#endif //PCH_H
