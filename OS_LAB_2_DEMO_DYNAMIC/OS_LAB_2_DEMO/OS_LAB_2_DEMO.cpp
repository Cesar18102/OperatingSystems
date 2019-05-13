#include "stdafx.h"
#include "resource.h"
#include <atlstr.h>

#define uint unsigned

typedef uint (*GetEncryptAndDecryptKeys)(uint* E, uint* D);
typedef uint (*Encrypt)(TCHAR* Msg, uint E, uint n, uint* EncMsg);
typedef void (*Decrypt)(uint* Msg, uint L, uint D, uint n, TCHAR* DecMsg);

int _tmain(int argc, _TCHAR* argv[]) {

	HMODULE hModule = LoadLibrary(_T("OS_LAB_2_LIB.dll"));

	GetEncryptAndDecryptKeys GetEncNDecKeys = (GetEncryptAndDecryptKeys)GetProcAddress(hModule, "?GetEncryptAndDecryptKeys@@YAIPAI0@Z");
	Encrypt Enc = (Encrypt)GetProcAddress(hModule, "?Encrypt@@YAIPA_WIIPAI@Z");
	Decrypt Dec = (Decrypt)GetProcAddress(hModule, "?Decrypt@@YAXPAIIIIPA_W@Z");

	uint* E = new uint();
	uint* D = new uint();
	uint n = GetEncNDecKeys(E, D);

	TCHAR* Msg = new TCHAR[256];
	uint* EncMsg = new uint[256];
	TCHAR* DecMsg = new TCHAR[256];

	Print_(_T("%s"), _T("Select language: Russian(R) or English(E): "));

	char* lang = new char();
	int LANG = LANG_ENGLISH;

	Read_(_T("%c"), lang);

	if(*lang == 'R' || *lang == 'r') {

		setlocale(LC_ALL, "Russian");
		LANG = LANG_RUSSIAN;

	} else if(*lang == 'E' || *lang == 'e') {

		setlocale(LC_ALL, "English");
		LANG = LANG_ENGLISH;
	}

	HMODULE mod = GetModuleHandle(NULL);
	CString str = CString();

	str.LoadStringW(mod, Input, LANG);
	Print_(_T("%s: "), str);
	Read_(_T("%s"), Msg);
	
	uint L = Enc(Msg, *E, n, EncMsg);

	str.LoadStringW(mod, Encode, LANG);
	Print_(_T("%s: "), str);
	Print_(_T("%d\n"), EncMsg);

	Dec(EncMsg, L, *D, n, DecMsg);

	str.LoadStringW(mod, Output, LANG);
	Print_(_T("%s: "), str);
	Print_(_T("%s\n"), DecMsg);

	system("pause");
	return 0;
}