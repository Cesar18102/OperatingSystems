#include "stdafx.h"

#ifdef UNICODE
#define read(F, X) wscanf(_T(F), X)
#define print(F, X) wprintf(_T(F), X)
#define compare(X, Y) _wcsicmp((wchar_t*)X, (wchar_t*)Y)
#define strend L'\0'
#else
#define read(F, X) scanf(F, X)
#define print(F, X) printf(F, X)
#define compare(X, Y) _stricmp((char*)X, (char*)Y)
#define strend '\0'
#endif

bool isUnicodeDefault() {

	return sizeof(TCHAR) == 2;
}

TCHAR* StrToGeneral(void* S, size_t L) {

	TCHAR* S_ = new TCHAR[L];
	int R = -1;

	bool StrUnicode = IsTextUnicode(S, L, &R);
	bool DefUnicode = isUnicodeDefault();

	if(StrUnicode && !DefUnicode) 
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)S, L, (char*)S_, L, NULL, NULL);
	else if(!StrUnicode && DefUnicode)
		MultiByteToWideChar(CP_ACP, 0, (char*)S, L, (wchar_t*)S_, L);
	else
		S_ = (TCHAR*)S;

	return S_;
}

int StrCompare(void* S1, size_t L1, void* S2, size_t L2) {

	TCHAR* S1_ = StrToGeneral(S1, L1);
	TCHAR* S2_ = StrToGeneral(S2, L2);

	return compare(S1_, S2_);
}

int IndexOf(TCHAR* S, size_t L, 
	  	    TCHAR* SubS, size_t SubL) {

	for(int i = 0; i <= L - SubL; i++) {

		bool Sub = true;
		for(int j = 0; j < SubL; j++)
			if(S[i + j] != SubS[j]) {

				Sub = false;
				break;
			}

		if(Sub)
			return i;
	}

	return -1;
}

TCHAR* ReplaceAt(TCHAR* S, size_t L, 
			     int SubI, size_t SubL,
			     TCHAR* Ins, size_t InsL) {
	
	if(SubI != -1) {

		size_t RL = L - SubL + InsL;
		TCHAR* Result = new TCHAR[RL];

		for(int j = 0; j < SubI; j++)
			Result[j] = S[j];

		for(int j = 0; j < InsL; j++)
			Result[SubI + j] = Ins[j];

		for(int j = 0; j < L - SubI - InsL; j++)
			Result[j + SubI + InsL] = S[j + SubI + SubL];

		Result[RL] = strend;
		return Result;
	}

	TCHAR* Result = new TCHAR[L];
	for(int i = 0; i < L; i++)
		Result[i] = S[i];

	Result[L] = strend;
	return Result;
}

TCHAR* ReplaceAll(TCHAR* S, size_t &L, 
			      TCHAR* Sub, size_t SubL,
			      TCHAR* Ins, size_t InsL) {

	int i = IndexOf(S, L, Sub, SubL);

	while(i != -1) {

		S = ReplaceAt(S, L, i, SubL, Ins, InsL);
		L = L - SubL + InsL;
		i = IndexOf(S, L, Sub, SubL);
	}

	return S;
}

TCHAR* SlashTrim(TCHAR* S, size_t &L) {

	if(S[L - 1] == '/') {

		S[L - 1] = '\0';
		L--;
	}

	return S;
}

size_t strlen(TCHAR* S, size_t maxSize) {

	size_t i = 0;
	for( ; i < maxSize && S[i] != strend; i++);
	return i;
}

void Task1() {

	const int MaxLen = 256;

	TCHAR* Path1 = new TCHAR[MaxLen];
	TCHAR* Path2 = new TCHAR[MaxLen];

	print("%s\n\n", _T("********************TASK1********************"));
	print("%s", _T("Input first path: "));
	read("%s", Path1);

	print("%s", _T("Input second path: "));
	read("%s", Path2);

	size_t L1 = strlen(Path1, MaxLen);
	size_t L2 = strlen(Path2, MaxLen);

	Path1 = ReplaceAll(Path1, L1, _T("\\"), 1, _T("/"), 1);
	Path1 = ReplaceAll(Path1, L1, _T("//"), 2, _T("/"), 1);
	Path1 = SlashTrim(Path1, L1);

	Path2 = ReplaceAll(Path2, L2, _T("\\"), 1, _T("/"), 1);
	Path2 = ReplaceAll(Path2, L2, _T("//"), 2, _T("/"), 1);
	Path2 = SlashTrim(Path2, L2);

	int Comparsion = StrCompare(Path1, L1, Path2, L2);
	print("%s\n\n", Comparsion == 0? _T("PATHES ARE EQUAL") : _T("PATHES ARE DIFFERENT"));
	print("%s\n\n", _T("********************TASK1********************"));
}

void Task2() {

	const int MaxLength = 256;
	bool Unicode = isUnicodeDefault();

	print("%s\n\n", _T("********************TASK2********************"));
	print("%s\n", Unicode? _T("Using UNICODE") : _T("Using ASCII"));

	int count;
	print("%s", _T("Input count of strings: "));
	read("%d", &count);

	if(Unicode) {

		wchar_t** strs = new wchar_t* [count];

		for(int i = 0; i < count; i++) {

			print("%s", _T("Input a string: "));
			strs[i] = new wchar_t[MaxLength];
			read("%s", strs[i]);
		}

		print("%s", "\n");

		for(int i = 0; i < count - 1; i++)
			for(int j = 0; j < count - 1; j++)
				if(_wcsicmp(strs[j], strs[j + 1]) > 0) {

					wchar_t* temp = strs[j];
					strs[j] = strs[j + 1];
					strs[j + 1] = temp;
				}

		for(int i = 0; i < count; i++)
			print("%s\n", strs[i]);

	} else {

		char** strs = new char* [count];

		for(int i = 0; i < count; i++) {

			print("%s", _T("Input a string: "));
			strs[i] = new char[MaxLength];
			read("%s", strs[i]);
		}

		print("%s", "\n");

		for(int i = 0; i < count - 1; i++)
			for(int j = 0; j < count - 1; j++)
				if(_stricmp(strs[j], strs[j + 1]) > 0) {

					char* temp = strs[j];
					strs[j] = strs[j + 1];
					strs[j + 1] = temp;
				}

		for(int i = 0; i < count; i++)
			print("%s\n", strs[i]);
	}

	print("\n%s\n\n", _T("********************TASK2********************"));
}

void Task3() {

	const int MaxLength = 256;
	print("%s\n\n", _T("********************TASK3********************"));

	int count;
	print("%s", _T("Input count of strings: "));
	read("%d", &count);

	TCHAR** strs = new TCHAR* [count];

	for(int i = 0; i < count; i++) {

		print("%s", _T("Input a string: "));
		strs[i] = new TCHAR[MaxLength];
		read("%s", strs[i]);
	}

	print("%s", "\n");

	for(int i = 0; i < count - 1; i++)
		for(int j = 0; j < count - 1; j++)
			if(compare(strs[j], strs[j + 1]) > 0) {

				TCHAR* temp = strs[j];
				strs[j] = strs[j + 1];
				strs[j + 1] = temp;
			}

	for(int i = 0; i < count; i++)
		print("%s\n", strs[i]);

	print("\n%s\n", _T("********************TASK3********************"));
}

int _tmain(int argc, _TCHAR* argv[]) {

	Task1();
	Task2();
	Task3();

	system("pause");
	return 0;
}

