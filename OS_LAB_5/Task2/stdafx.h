// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <locale>

#ifdef UNICODE
#define Print wprintf
#else
#define Print printf
#endif

// TODO: reference additional headers your program requires here
