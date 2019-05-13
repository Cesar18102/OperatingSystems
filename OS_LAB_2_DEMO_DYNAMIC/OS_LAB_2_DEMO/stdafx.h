#pragma once

#include "targetver.h"

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <random>

#ifdef UNICODE
#define Read_ wscanf
#define Print_ wprintf
#define Length_ lstrlenW
#else
#define Read_ scanf
#define Print_ printf
#define Length_ strlen
#endif