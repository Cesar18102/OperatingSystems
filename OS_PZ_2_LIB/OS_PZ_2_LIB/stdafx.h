// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifdef UNICODE
#define Read(F, X) wscanf(_T(F), X)
#define Print(F, X) wprintf(_T(F), X)
#define Length lstrlenW
#define ToStr _itow
#else
#define Read(F, X) scanf(_T(F), X)
#define Print(F, X) printf(_T(F), X)
#define Length strlen
#define ToStr itoa
#endif

// Windows Header Files:

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>



// TODO: reference additional headers your program requires here
