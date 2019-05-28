#include "stdafx.h"

void PrintFileData(WIN32_FIND_DATA F) {

	Print(_T("File %s is now processed\n"), F.cFileName);
	
	HANDLE h = CreateFile(F.cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0); 
	DWORD size = GetFileSize(h, NULL);

	BYTE *data = new BYTE[size];
	DWORD *haveRead = new DWORD(0);
	ReadFile(h, data, size, haveRead, NULL);

	Print(_T("Total file size: %d\n"), size);

	wchar_t *UnicodeData = new wchar_t[size];
	MultiByteToWideChar(CP_ACP, NULL, (char*)data, size, UnicodeData, size);
	UnicodeData[size] = L'\0';

	int lineCount = 0;
	int lineLen = 0;

	for(int i = 0; i < size - 1; i++) {
		if(UnicodeData[i] == L'\r' && UnicodeData[i + 1] == L'\n') {
			Print(_T("Line #%d consists of %d symbols\n"), ++lineCount, lineLen);
			lineLen = 0;
		}
		if(UnicodeData[i] != L'\r' && UnicodeData[i] != L'\n')
			lineLen++;
	}

	Print(_T("Line #%d consists of %d symbols\n"), ++lineCount, ++lineLen);
	Print(_T("Total line count is %d\n"), lineCount);

	Print(_T("\n==================================\n\n"));
}

void Task2() {

	WIN32_FIND_DATA FindFileData;
	HANDLE Found;

	Found = FindFirstFile(_T("*.txt"), &FindFileData);
	if (Found == INVALID_HANDLE_VALUE) {
		Print(_T("No files were in project dir\n"));
		return;
	} else {
		do { PrintFileData(FindFileData); }
		while(FindNextFile(Found, &FindFileData));
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	Task2();
	return 0;
}

