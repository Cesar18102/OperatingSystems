#include "stdafx.h"

typedef struct PROCESS{

	PROCESSENTRY32W Prc;
	DWORD CurPrior;

	PROCESS(PROCESSENTRY32W P) {

		Prc = P;
		CurPrior = P.pcPriClassBase;
	}

	PROCESS() { CurPrior = 0; }
};

int AllocTime(PROCESS* Processes, int count) {

	int maxPriorId = 0;
	for(int i = 1; i < count; i++)
		if(Processes[i].CurPrior > Processes[maxPriorId].CurPrior)
			maxPriorId = i;

	Processes[maxPriorId].CurPrior = Processes[maxPriorId].Prc.pcPriClassBase;
	for(int i = 0; i < count; i++)
		if(i != maxPriorId)
			Processes[i].CurPrior++;

	return maxPriorId;
}

int _tmain(int argc, _TCHAR* argv[]) {

	PROCESS* Processes = new PROCESS[1024];
	int count = 0;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W entry = PROCESSENTRY32W();
	entry.dwSize = sizeof(entry);

	if (!Process32FirstW(snap, &entry))
		return 0;

	do  { Processes[count++] = PROCESS(entry); } 
	while (Process32NextW(snap, &entry));

	int pause = 100;
	while(true) {

		int id = AllocTime(Processes, count);
		wprintf(_T("Process %s recieved a quant of time\n"), Processes[id].Prc.szExeFile);
		Sleep(pause);
	}

	system("pause");
	return 0;
}

