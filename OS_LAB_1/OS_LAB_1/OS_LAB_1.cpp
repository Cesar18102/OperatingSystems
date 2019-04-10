#include "stdafx.h"

#define ENCODING char*
#define ASCII "ASCII"
#define UCODE "UNICODE"

#ifdef UNICODE
#define length lstrlenW
#define ifstr std::wifstream
#define print_str(X) wprintf(L"%s\n", X);
#else
#define length strlen
#define ifstr std::ifstream
#define print_str(X) printf("%s\n", X);
#endif

ENCODING getDefaultEncoding() {

	return sizeof(TCHAR) == 1 ? ASCII : UCODE;
}

int wstrComparer(const void* S1, const void* S2) {

	return _wcsicmp((wchar_t*) S1, (wchar_t*) S2);
}

void Reverse(TCHAR* str) {

	int len = length(str);
	for(int i = 0; i < len / 2; i++) {

		TCHAR temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}
}

int getPiecesCount(TCHAR* str, TCHAR delim) {

	int piece_count = 1;
	int len = length(str);

	for(int i = 0; i < len; i++)
		if(str[i] == delim)
			piece_count++;

	return piece_count;
}

void Split(TCHAR* str, TCHAR delim, TCHAR** pieces, int piece_count) {

	int len = length(str);

	for(int i = 0, n = 0; n < piece_count && i < len; i++, n++) {

		int piece_len = 0;
		for( ; i + piece_len < len && str[i + piece_len] != delim; piece_len++);
		
		pieces[n] = new TCHAR[len];
		for(int j = 0; j < piece_len; j++, i++)
			pieces[n][j] = str[i];

		pieces[n][piece_len] = _T('\0');
	}
}

int getSumLength(TCHAR** pieces, int piece_count) {

	int sumlen = 0;
	for(int i = 0; i < piece_count; i++)
		sumlen += length(pieces[i]) + 1;

	return sumlen;
}

void Join(TCHAR** pieces, TCHAR delim, TCHAR* str, int piece_count, int sumlen) {

	for(int i = 0, n = 0; n < piece_count && i < sumlen; i++, n++) {

		int piece_len = length(pieces[n]);

		for(int j = 0; j < piece_len; i++, j++)
			str[i] = pieces[n][j];

		str[i] = delim;
	}

	str[sumlen] = _T('\0');
}

bool isTextUnicode(TCHAR* str) {//FEFF
								//FFEF
	byte C1 = (byte)str[0];
	byte C2 = (byte)str[1];

	byte F = 0xFF;
	byte E = 0xFE;

	bool B1 = C1 == E && C2 == F;
	bool B2 = C1 == F && C2 == E;

	return B1 || B2;
}

void LastTask(TCHAR* filename, TCHAR delim) {

	ifstr istr;

	istr.open(filename);

	istr.seekg(0, std::ios::end);
    int fileSize = istr.tellg();

    istr.close();

	istr.open(filename);

	TCHAR* input = new TCHAR[fileSize];
	istr >> input;

	istr.close();

	TCHAR* str = new TCHAR[fileSize];

	int enc = 0;
	char* ENC = getDefaultEncoding();
	bool isUnicode = IsTextUnicode(input, fileSize, &enc);

	if(isUnicode && ENC == ASCII)
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)input, fileSize, (char*)str, fileSize, NULL, NULL);
	else if(!isUnicode && ENC == UCODE)
		MultiByteToWideChar(CP_ACP, 0, (char*)input, fileSize, (wchar_t*)str, fileSize);
	else 
		str = input;

	int PC = getPiecesCount(str, delim);

	TCHAR** pieces = new TCHAR* [PC];
	Split(str, delim, pieces, PC);

	for(int i = 0; i < PC; i++)
		Reverse(pieces[i]);

	int SL = getSumLength(pieces, PC);

	TCHAR* backstr = new TCHAR[SL];
	Join(pieces, delim, backstr, PC, SL);

	print_str(backstr);
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

	printf("\n---------LAST TASK--------\n\n");
	LastTask(_T("D:/!!!OLEG/!!!WORK/GitHubProjects/OperatingSystems/OS_LAB_1/Debug/1.txt"), _T(';'));

	system("pause");
	return 0;
}