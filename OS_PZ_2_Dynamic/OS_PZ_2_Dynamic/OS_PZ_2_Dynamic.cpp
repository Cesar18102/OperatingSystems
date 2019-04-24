#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hInst = LoadLibrary(_T("OS_PZ_2_LIB.dll"));

	ReadBigInt BigIntReader = (ReadBigInt)GetProcAddress(hInst, "?ReadBigInt@@YA?AVBigInt@@XZ");
	PrintBigInt BigIntPrinter = (PrintBigInt)GetProcAddress(hInst, "?PrintBigInt@@YAXVBigInt@@@Z");
	Adder BigIntAdd = (Adder)GetProcAddress(hInst, "?Plus@@YA?AVBigInt@@V1@0@Z");
	Substractor BigIntSubstract = (Substractor)GetProcAddress(hInst, "?Minus@@YA?AVBigInt@@V1@0@Z");
	Multiplier BigIntMultiply = (Multiplier)GetProcAddress(hInst, "?Multiply@@YA?AVBigInt@@V1@0@Z");
	
	BigInt A = BigIntReader();
	BigInt B = BigIntReader();

	BigInt Sum = BigIntAdd(A, B);
	BigInt Sub = BigIntSubstract(A, B);
	BigInt Mul = BigIntMultiply(A, B);

	BigIntPrinter(A);
	std::cout << " + ";
	BigIntPrinter(B);
	std::cout << " = ";
	BigIntPrinter(Sum);
	std::cout << std::endl;

	BigIntPrinter(A);
	std::cout << " - ";
	BigIntPrinter(B);
	std::cout << " = ";
	BigIntPrinter(Sub);
	std::cout << std::endl;

	BigIntPrinter(A);
	std::cout << " * ";
	BigIntPrinter(A);
	std::cout << " = ";
	BigIntPrinter(Mul);
	std::cout << std::endl;

	system("pause");
	return 0;
}