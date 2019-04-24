#include "stdafx.h"
#include "OS_PZ_2_LIB.h"

EIAPI BigInt ReadBigInt() {
	
	TCHAR* a = new TCHAR[256];
	Read("%s", a);
	return BigInt::GetBigInt(a);
}

EIAPI void PrintBigInt(BigInt A) {

	A.print();
}

EIAPI BigInt CreateBigInt(short* digits, int digitsCount, bool minus) {

	return BigInt(digits,digitsCount, minus);
}

EIAPI BigInt Plus(BigInt A, BigInt B) {

	return A + B;
}

EIAPI BigInt Minus(BigInt A, BigInt B) {

	return A - B;
}

EIAPI BigInt Multiply(BigInt A, BigInt B) {

	return A * B;
}
