#include "stdafx.h"

typedef bool (*CheckInteg)(HANDLE);

int _tmain(int argc, _TCHAR* argv[]) {

	HANDLE h = CreateFile(_T("C:\\Users\\Юыху\\Documents\\Visual Studio 2012\\Projects\\OperatingSystems\\OS_PZ_3_TEST\\Debug\\OS_PZ_3_DLL.dll"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	HMODULE hMod = LoadLibrary(_T("../OS_PZ_3_DLL.dll"));
	CheckInteg CI = (CheckInteg)GetProcAddress(hMod, "?CheckIntegrity@@YA_NPAX@Z");
	std::cout << (CI(h) ? "Library is integrable" : "Library is NOT integrable") << std::endl;
	system("pause");
	return 0;
}