#include "stdafx.h"

#ifdef OS_PZ_3_DLL_EXPORTS
#define EIAPI __declspec(dllexport)
#else
#define EIAPI __declspec(dllimport)
#endif

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

EIAPI bool CheckIntegrity(HANDLE h) {

	DWORD size = GetFileSize(h, 0);
	DWORD CheckSumSize = sizeof(DWORD);

	if (h == INVALID_HANDLE_VALUE)
		return Results::FILE_NOT_EXIST;

	PBYTE fileContent = new BYTE[size - CheckSumSize];
	DWORD readBytesCount, controlSumReal;
	ReadFile(h, fileContent, size - CheckSumSize, &readBytesCount, 0); 
	controlSumReal = GetControlSum(fileContent, size - CheckSumSize); 

	PBYTE checkSumFromFile = new BYTE[CheckSumSize];
	DWORD controlSumWritten = 0;
	ReadFile(h, checkSumFromFile, CheckSumSize, &readBytesCount, 0);
	memcpy(&controlSumWritten, checkSumFromFile, CheckSumSize);

	CloseHandle(h);

	return controlSumReal == controlSumWritten;
}