#include "stdafx.h"

enum Results {

	FILE_NOT_EXIST,
	READING_ERROR,
	OK
};

DWORD GetControlSum(PBYTE data, DWORD size){ 

	DWORD controlSum = 0; 
	DWORD sizeCompressed4b = size / 4; 
	PDWORD dataCompressed4b = (PDWORD)data; 

	for (DWORD i = 0; i < sizeCompressed4b; ++i) 
		controlSum += dataCompressed4b[i];

	DWORD restSize = size % 4; 

	if (restSize) { 

		DWORD dwNumber = 0; 
		memcpy(&dwNumber, data, restSize); 
		controlSum += dwNumber; 
	}

	return controlSum; 
} 

DWORD GetControlSumForFile(TCHAR* filename) { 

	HANDLE h = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (h == INVALID_HANDLE_VALUE)
		return Results::FILE_NOT_EXIST;

	DWORD fileSize = GetFileSize(h, 0);
	PBYTE fileContent = new BYTE[fileSize];
	DWORD readBytesCount, controlSum; 
	BOOL b = ReadFile(h, fileContent, fileSize, &readBytesCount, 0); 

	if (b) { 

		controlSum = GetControlSum(fileContent, fileSize); 
		b = WriteFile(h, &controlSum, sizeof(controlSum), &readBytesCount, 0); 
	}

	delete[] fileContent; 
	CloseHandle(h); 

	return b ? Results::OK : Results::READING_ERROR; 
}

int _tmain(int argc, _TCHAR* argv[]) {

	DWORD d = GetControlSumForFile(_T("../OS_PZ_3_DLL.dll"));
	return 0;
}