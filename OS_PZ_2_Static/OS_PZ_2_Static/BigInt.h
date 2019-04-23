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

class __declspec(dllexport) BigInt
{
	public:
		BigInt(short* digits, long long len, bool minus);
		BigInt();
		~BigInt(void);
		void print();

		BigInt operator + (BigInt B);
		BigInt operator - (BigInt B);
		BigInt operator - ();
		BigInt operator * (BigInt B);
		BigInt operator / (BigInt B);
		static BigInt ReadBigInt();
		static BigInt GetBigInt(TCHAR* ds);
		friend __declspec(dllexport) std::ostream& BigInt::operator << (std::ostream& os, const BigInt& B);

	private:
		short* digits;
		int digitsCount;
		bool minus;
};