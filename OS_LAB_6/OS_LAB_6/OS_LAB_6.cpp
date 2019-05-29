#include "stdafx.h"

using namespace std;

int TCount = 10;
int *calls = new int[TCount];
int lastCalled = -1;

DWORD WINAPI Foo1(LPVOID T) {

	DWORD Tid = *(DWORD*)T;
	printf("Thread with id %d created\n", Tid + 1); 

	for(int i = 0; i < 999999; i++) //9999
		if(lastCalled != Tid) {
			lastCalled = Tid;
			calls[Tid]++;
		}

	printf("Thread with id %d finished working\n", Tid + 1);
	return 0;
}

DWORD WINAPI T1(LPVOID T) {

	HANDLE *THDS = new HANDLE[TCount];
	DWORD *Tid = new DWORD[TCount];

	for(int i = 0; i < TCount; i++)
		calls[i] = 0;

	for(int i = 0; i < TCount; i++)
		THDS[i] = CreateThread(NULL, 4096 * 1024, Foo1, new int(i), NULL, &Tid[i]);

	WaitForMultipleObjects(TCount, THDS, true, INFINITE);

	printf("\n\n");
	for(int i = 0; i < TCount; i++)
		printf("Thread %d was switched for %d times\n", i + 1, calls[i]);

	return 0;
}

DWORD WINAPI Foo2(LPVOID T) {
	(*(int*)T)++;
	for(int i = 0; i < 99999999; i++);
	return 0;
}

PROCESSENTRY32W GetCurrentProcessEntry() {

	HANDLE h = GetCurrentProcess();
	DWORD id = GetProcessId(h);

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snap, &entry);

	while(entry.th32ProcessID != id)
		Process32Next(snap, &entry);

	return entry;
}

DWORD WINAPI T2(LPVOID T) {

	for(int i = 2; ; i *= 2) {

		printf("Trying to start %d threads\n", i);

		int count = 0;
		HANDLE *THDS = new HANDLE[i];
		DWORD *Tid = new DWORD[i];

		for(int j = 0; j < i; j++)
			THDS[j] = CreateThread(NULL, 4096 * 1024, Foo2, &count, NULL, &Tid[i]);

		PROCESSENTRY32W entry = GetCurrentProcessEntry();
		printf("Real threads count is %d\n\n", entry.cntThreads);

		WaitForMultipleObjects(i, THDS, true, INFINITE);
		if(count != i) {
			printf("\n\nStart %d threads was failed.\n%d threads is maximum count\n", i, i);
			break;
		}
	}

	return 0;
}

bool IsUnicodeDefault() {
	return sizeof(TCHAR) != 1;
}

class Reader {

public:
	TCHAR *Name;

	Reader() { }

	Reader(TCHAR *name, TCHAR *filename) : Name(name), Filename(filename) {
		File = CreateFile(Filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
		FilePointer = 0;
	}

	DWORD GetNewSize() {
		return GetFileSize(File, NULL) - FilePointer;
	}

	DWORD ReadNext() {
		BufferSize = GetNewSize();
		if(BufferSize == 0) return -1; //if there is any new

		Buffer = new BYTE[BufferSize];
		DWORD BytesRead = FilePointer;
		ReadFile(File, Buffer, BufferSize, &BytesRead, NULL);
		FilePointer += BytesRead;
		return 0;
	}

	DWORD GetBufferSize() { return BufferSize; }

	void GetBuffer(TCHAR *buffer) {
		if(IsUnicodeDefault())
			MultiByteToWideChar(CP_ACP, NULL, (char*)Buffer, BufferSize, buffer, BufferSize);
		else
			buffer = (TCHAR*)Buffer;
		buffer[BufferSize] = _T('\0');
	}

private:
	DWORD FilePointer;
	TCHAR *Filename;
	BYTE *Buffer;
	DWORD BufferSize;
	HANDLE File;
};

DWORD WINAPI ReadWait(LPVOID T) {
	while(((Reader*)T)->ReadNext() == -1)
		Sleep(100);
	return 0;
}

DWORD WINAPI ReadAlways(LPVOID T) {
	while(true) {
		ReadWait(T);

		Reader *R = (Reader*)T;
		DWORD size = R->GetBufferSize();
		TCHAR *buffer = new TCHAR[size];
		R->GetBuffer(buffer);

		_tprintf(_T("Reader %s reads: %s\n"), R->Name, buffer);
	}
}

class Writer {

public:
	TCHAR *Name;

	Writer() { }

	Writer(TCHAR *name, TCHAR *filename) : Name(name), Filename(filename) {
		File = CreateFile(Filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	}

	void Write(TCHAR *data, DWORD size) {

		DWORD EndOfFile = GetFileSize(File, NULL);
		BYTE *buffer = new BYTE[size];

		if(IsUnicodeDefault())
			WideCharToMultiByte(CP_ACP, NULL, data, size, (char*)buffer, size, NULL, NULL);
		else
			buffer = (BYTE*)data;

		WriteFile(File, buffer, size, &EndOfFile, NULL);
	}

private:
	TCHAR *Filename;
	HANDLE File;
};

DWORD WINAPI Write(LPVOID T) {

	Writer *W = (Writer*)T;

	TCHAR *Temp = _T(" wrote a new\n");
	DWORD TempSize = Length(Temp);
	DWORD NameSize = Length(W->Name);

	DWORD size = NameSize + TempSize;
	TCHAR *message = new TCHAR[size];

	StrCp(message, W->Name);
	StrCp(message + NameSize, Temp);
	message[size] = _T('\0');

	W->Write(message, size);
	Print(_T("%s\n"), message);

	return 0;
}

DWORD WINAPI WriteAlways(LPVOID T) {

	while(true) {
		Write(T);
		srand(time(NULL));
		Sleep(100 + rand() % 400);
	}
}

DWORD WINAPI T3(LPVOID T) {

	int ReadCount = 5;
	int WriteCount = 5;

	Reader R1 = Reader(_T("Reader1"), _T("test1.txt"));
	Reader R2 = Reader(_T("Reader2"), _T("test1.txt"));
	Reader R3 = Reader(_T("Reader3"), _T("test1.txt"));
	Reader R4 = Reader(_T("Reader4"), _T("test1.txt"));
	Reader R5 = Reader(_T("Reader5"), _T("test1.txt"));

	Writer W1 = Writer(_T("Writer1"), _T("test1.txt"));
	Writer W2 = Writer(_T("Writer2"), _T("test1.txt"));
	Writer W3 = Writer(_T("Writer3"), _T("test1.txt"));
	Writer W4 = Writer(_T("Writer4"), _T("test1.txt"));
	Writer W5 = Writer(_T("Writer5"), _T("test1.txt"));

	Reader RS[] = { R1, R2, R3, R4, R5 };
	HANDLE *ReadTHs = new HANDLE[ReadCount];
	DWORD *ReadIDs = new DWORD[ReadCount];

	Writer WS[] = { W1, W2, W3, W4, W5 };
	HANDLE *WriteTHs = new HANDLE[WriteCount];
	DWORD *WriteIDs = new DWORD[WriteCount];

	for(int i = 0; i < WriteCount; i++)
		WriteTHs[i] = CreateThread(NULL, 4096 * 1024, WriteAlways, &WS[i], NULL, &WriteIDs[i]);

	for(int i = 0; i < ReadCount; i++)
		ReadTHs[i] = CreateThread(NULL, 4096 * 1024, ReadAlways, &RS[i], NULL, &ReadIDs[i]);

	WaitForMultipleObjects(WriteCount, WriteTHs, true, INFINITE);
	WaitForMultipleObjects(ReadCount, ReadTHs, true, INFINITE);

	return 0;
}

class Philosopher {

public:
	TCHAR *Name;

	Philosopher(TCHAR *name) : Name(name) { }

	void Think() {
		Print(_T("%s is thinking\n"), Name);
		Sleep(1000);
		Print(_T("%s has finished thinking\n"), Name);
	}

	void Lunch() {
		Print(_T("%s is having a lunch\n"), Name);
		Sleep(1000);
		Print(_T("%s has finished having a lunch\n"), Name);
	}

	void TakeLeftFork() {
		Print(_T("%s is now taking left fork\n"), Name);
		Sleep(20);
		Print(_T("%s has took left fork\n"), Name);
	}

	void TakeRightFork() {
		Print(_T("%s is now taking right fork\n"), Name);
		Sleep(20);
		Print(_T("%s has took right fork\n"), Name);
	}

	void DropLeftFork() {
		Print(_T("%s is now dropping left fork\n"), Name);
		Sleep(20);
		Print(_T("%s has dropped left fork\n"), Name);
	}

	void DropRightFork() {
		Print(_T("%s is now dropping right fork\n"), Name);
		Sleep(20);
		Print(_T("%s has dropped right fork\n"), Name);
	}
};

DWORD WINAPI PhilosopherLunch(LPVOID T) {
	Philosopher *P = (Philosopher*)T;
	for(int i = 0; i < 10; i++) {
		P->Think();
		P->TakeLeftFork();
		P->TakeRightFork();
		P->Lunch();
		P->DropRightFork();
		P->DropLeftFork();
		P->Think();
	}
	return 0;
}

DWORD WINAPI T4(LPVOID T) {

	int Count = 5;

	Philosopher P1 = Philosopher(_T("Kant"));
	Philosopher P2 = Philosopher(_T("Gegel"));
	Philosopher P3 = Philosopher(_T("Nitshe"));
	Philosopher P4 = Philosopher(_T("Dekart"));
	Philosopher P5 = Philosopher(_T("Pyfagor"));

	Philosopher PS[] = { P1, P2, P3, P4, P5 };
	HANDLE *THDs = new HANDLE[Count];
	DWORD *IDs = new DWORD[Count];

	for(int i = 0; i < Count; i++)
		THDs[i] = CreateThread(NULL, 4096 * 1024, PhilosopherLunch, &PS[i], NULL, &IDs[i]);

	WaitForMultipleObjects(Count, THDs, true, INFINITE);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {

	T1(NULL);
	T2(NULL);
	T3(NULL);
	T4(NULL);
	/*int Count = 4;

	HANDLE *THDs = new HANDLE[Count];
	DWORD *IDs = new DWORD[Count];

	THDs[0] = CreateThread(NULL, 4096 * 1024, T1, NULL, NULL, &IDs[0]);
	THDs[1] = CreateThread(NULL, 4096 * 1024, T2, NULL, NULL, &IDs[1]);
	THDs[2] = CreateThread(NULL, 4096 * 1024, T3, NULL, NULL, &IDs[3]);
	THDs[3] = CreateThread(NULL, 4096 * 1024, T4, NULL, NULL, &IDs[4]);

	WaitForMultipleObjects(Count, THDs, true, INFINITE);*/

	system("pause");
	return 0;
}

