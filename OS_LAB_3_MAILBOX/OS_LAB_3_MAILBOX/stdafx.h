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
#define Length lstrlenW
#define Copy lstrcpyW
#else
#define Length strlen
#deinfe Copy strcpy
#endif