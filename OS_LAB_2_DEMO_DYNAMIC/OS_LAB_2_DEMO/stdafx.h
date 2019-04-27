#pragma once

#include "targetver.h"

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <random>

#ifdef UNICODE
#define Read wscanf
#define Print wprintf
#define Length lstrlenW
#else
#define Read scanf
#define Print printf
#define Length strlen
#endif