#include "stdafx.h"

using namespace std;

void PrintProcessInfo(STARTUPINFO StUpInfo, PROCESS_INFORMATION PI) {

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snap, &entry);

	while(entry.th32ProcessID != PI.dwProcessId)
		Process32Next(snap, &entry);

	Print(_T("Process %s started\n"), entry.szExeFile);

	HMODULE DLLs[1024];
	DWORD DLLBytes = 0;
	EnumProcessModules(PI.hProcess, DLLs, sizeof(DLLs), &DLLBytes);

	DWORD DLLCount = DLLBytes / sizeof(HMODULE);
	Print(_T("Total DLL count %d\nDLL List: "), DLLCount);

	for(int i = 0; i < DLLCount; i++) {
		TCHAR *DLLName = new TCHAR[1024];
		GetModuleBaseName(PI.hProcess, DLLs[i], DLLName, 1024);
		Print(_T("%s, "), DLLName);
	}

	Print(_T("\nTotal threads count: %d\n\n\n"), entry.cntThreads);
}

int _tmain(int argc, _TCHAR* argv[]) {

	const bool ASCII = false;

	STARTUPINFO StUpInfo1;
	ZeroMemory(&StUpInfo1, sizeof(STARTUPINFO));
	PROCESS_INFORMATION PI1;

	_CreateProcess(_T("..\\Debug\\Task1.exe"), (ASCII ? _T("/A") : _T("/W")), StUpInfo1, PI1);
	Sleep(50);
	PrintProcessInfo(StUpInfo1, PI1);
	WaitForSingleObject(PI1.hProcess, INFINITE);


	STARTUPINFO StUpInfo2;
	ZeroMemory(&StUpInfo2, sizeof(STARTUPINFO));
	PROCESS_INFORMATION PI2;

	_CreateProcess(_T("..\\Debug\\Task2.exe"), (ASCII ? _T("/A") : _T("/W")), StUpInfo2, PI2);
	Sleep(10);
	PrintProcessInfo(StUpInfo2, PI2);

	system("pause");
	return 0;
}

