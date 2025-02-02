#include "stdafx.h"

#define uint unsigned

typedef uint (*GetEncryptAndDecryptKeys)(uint* E, uint* D);
typedef uint (*Encrypt)(TCHAR* Msg, uint E, uint n, uint* EncMsg);
typedef void (*Decrypt)(uint* Msg, uint L, uint D, uint n, TCHAR* DecMsg);

void Task1(TCHAR *Encoding) {

	STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

	if(!_CreateProcess(_T("C:\\Windows\\System32\\notepad.exe"), Encoding, cif, pi)) {
		Print(_T("%s"), _T("Statring notepad.exe have occured an exception"));
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = LoadLibrary(_T("OS_LAB_2_LIB.dll"));

	GetEncryptAndDecryptKeys GetEncNDecKeys = (GetEncryptAndDecryptKeys)GetProcAddress(hModule, "?GetEncryptAndDecryptKeys@@YAIPAI0@Z");
	Encrypt Enc = (Encrypt)GetProcAddress(hModule, "?Encrypt@@YAIPA_WIIPAI@Z");
	Decrypt Dec = (Decrypt)GetProcAddress(hModule, "?Decrypt@@YAXPAIIIIPA_W@Z");

	Task1(argc == 0 ? _T("/A") : argv[0]);
	return 0;
}

