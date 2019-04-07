#include "stdafx.h"

#define ENCODING char*
#define ASCII "ASCII"
#define UCODE "UNICODE"

ENCODING getDefaultEncoding() {

	return sizeof(TCHAR) == 1 ? ASCII : UCODE;
}

int wstrComparer(const void* S1, const void* S2) {

	return _wcsicmp((wchar_t*) S1, (wchar_t*) S2);
}

int _tmain(int argc, _TCHAR* argv[]) {

	_tsetlocale(LC_ALL, _T("Russian"));

	printf("%s\n", getDefaultEncoding());
	printf("\n------------CHARs---------\n\n");

	const int FamilyCount = 4;
	char** Family = new char* [4];
	
	Family[0] = "Кириченко Владимир Викторович";
	Family[1] = "Кириченко Светлана Юрьевна";
	Family[2] = "Кириченко Олег Владимирович";
	Family[3] = "Кириченко Кирилл Владимирович";

	for(int i = 0; i < FamilyCount; i++)
		printf("%s\n", Family[i]);

	printf("\n----------WCHAR_Ts--------\n\n");

	wchar_t** WFamily = new wchar_t* [FamilyCount];

	for(int i = 0; i < FamilyCount; i++) {

		int S = strlen(Family[i]);
		WFamily[i] = new wchar_t[S];

		MultiByteToWideChar(CP_ACP, 0, Family[i], S, WFamily[i], S);
		WFamily[i][S] = '\0';

		wprintf(L"%s\n", WFamily[i]);
		MessageBoxW(0, WFamily[i], L"Family", MB_OK);
	}

	printf("\n--------CHARs AGAIN-------\n\n");

	char** BackFamily = new char* [FamilyCount];
	
	for(int i = 0; i < FamilyCount; i++) {

		int S = lstrlenW(WFamily[i]);
		BackFamily[i] = new char[S];

		WideCharToMultiByte(CP_ACP, 0, WFamily[i], S, BackFamily[i], S, NULL, NULL);
		BackFamily[i][S] = '\0';

		printf("%s\n", BackFamily[i]);
	}

	printf("\n--------------------------\n\n");

	system("pause");
	return 0;
}