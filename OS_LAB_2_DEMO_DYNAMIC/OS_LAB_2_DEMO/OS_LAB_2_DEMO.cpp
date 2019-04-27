#include "stdafx.h"
#define uint unsigned

#ifdef UNICODE
#define Length lstrlenW
#define TCHARMAX WCHAR_MAX
#else
#define Length strlen
#define TCHARMAX CHAR_MAX
#endif

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

	Read(_T("%s"), Msg);
	
	uint L = Enc(Msg, *E, n, EncMsg);
	Dec(EncMsg, L, *D, n, DecMsg);

	Print(_T("%s\n"), DecMsg);

	system("pause");
	return 0;
}