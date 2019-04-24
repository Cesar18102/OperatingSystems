#pragma once

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

class BigInt
{
	public:
		BigInt(short* digits, long long len, bool minus);
		BigInt();
		void print();

		BigInt operator + (BigInt B);
		BigInt operator - (BigInt B);
		BigInt operator - ();
		BigInt operator * (BigInt B);
		BigInt operator / (BigInt B);
		static BigInt GetBigInt(TCHAR* ds);
		friend std::ostream& BigInt::operator << (std::ostream& os, const BigInt& B);

	private:
		short* digits;
		int digitsCount;
		bool minus;
};