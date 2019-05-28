#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <locale>

#define _CreateProcess(Path, Args, CIF, PI) CreateProcess(Path, Args, NULL, NULL, FALSE, NULL, NULL, NULL, &CIF,&PI)
#define _CreateProcessSuspended(Path, Args, CIF, PI) CreateProcess(Path, Args, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &CIF, &PI)

#ifdef UNICODE
#define Print wprintf
#else
#define Print printf
#endif