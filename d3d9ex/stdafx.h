// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define NOMINMAX
#define STRICT
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <stdint.h>
#include <inttypes.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <mutex>
#include <memory>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>

#include <d3d9.h>

#include "WinUtil.h"
#include "Logger.h"