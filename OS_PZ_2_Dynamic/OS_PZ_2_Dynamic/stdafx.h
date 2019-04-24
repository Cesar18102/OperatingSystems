#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include "BigInt.h"

typedef BigInt (*ReadBigInt)(void);
typedef void (*PrintBigInt)(BigInt);
typedef BigInt (*Adder)(BigInt A, BigInt B);
typedef BigInt (*Substractor)(BigInt A, BigInt B);
typedef BigInt (*Multiplier)(BigInt A, BigInt B);
