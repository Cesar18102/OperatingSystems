// OS_LAB_2_DEMO_STATIC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OS_LAB_2_LIB.h"
#include <iostream>

#define uint unsigned

#ifdef UNICODE
#define Read wscanf
#define Print wprintf
#define Length lstrlenW
#else
#define Read scanf
#define Print printf
#define Length strlen
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	uint* E = new uint();
	uint* D = new uint();
	uint n = GetEncryptAndDecryptKeys(E, D);

	TCHAR* Msg = new TCHAR[256];
	uint* EncMsg = new uint[256];
	TCHAR* DecMsg = new TCHAR[256];

	Read(_T("%s"), Msg);
	
	uint L = Encrypt(Msg, *E, n, EncMsg);
	Decrypt(EncMsg, L, *D, n, DecMsg);

	Print(_T("%s\n"), DecMsg);

	system("pause");
	return 0;
}

