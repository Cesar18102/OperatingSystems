#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>

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