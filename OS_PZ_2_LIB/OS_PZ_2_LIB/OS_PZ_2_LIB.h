#pragma once
#ifdef OS_PZ_2_LIB_EXPORTS
#define EIAPI __declspec(dllexport)
#else
#define EIAPI __declspec(dllimport)
#endif

#include "BigInt.h"

EIAPI BigInt ReadBigInt();
EIAPI void PrintBigInt(BigInt A);
EIAPI BigInt CreateBigInt(short* digits, int digitsCount, bool minus);
EIAPI BigInt Plus(BigInt A, BigInt B);
EIAPI BigInt Minus(BigInt A, BigInt B);
EIAPI BigInt Multiply(BigInt A, BigInt B);