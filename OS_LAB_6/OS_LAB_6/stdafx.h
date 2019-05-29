#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <math.h>
#include <time.h>

#ifdef UNICODE
#define Length lstrlenW
#define StrCp lstrcpyW
#define Print wprintf
#else
#define Length strlen
#define StrCp strcpy
#define Print printf
#endif