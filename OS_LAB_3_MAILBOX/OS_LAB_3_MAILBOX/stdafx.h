// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h> 
#include <stdlib.h> 
#include <windows.h> 
#include <string.h> 

#ifdef UNICODE
#define Read_(F, X) wscanf(_T(F), X)
#define GRead_ _getws
#define Write_(F, X) wprintf(_T(F), (wchar_t*)X)
#define CWrite_(F, X) wprintf(_T(F), _T(X))
#define Length lstrlenW
#define Copy lstrcpyW
#else
#define Read_(F, X) scanf(_T(F), X)
#define GRead_ gets
#define Write_(F, X) printf(_T(F), (char*)X)
#define CWrite_(F, X) printf(_T(F), _T(X))
#define Length strlen
#define Copy strcpy
#endif