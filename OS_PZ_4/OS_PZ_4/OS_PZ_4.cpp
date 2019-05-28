#include "stdafx.h"

typedef struct PROCESS {

	PROCESSENTRY32W Prc;
	DWORD CurPrior;
	int QID;

	PROCESS(PROCESSENTRY32W P, int QID) {

		Prc = P;
		CurPrior = P.pcPriClassBase;
		this->QID = QID;
	}

	PROCESS() { CurPrior = 0; }
};

class Queue {

	public:	
		Queue() {
			PCS = new PROCESS[max];
		}

		int size() {
			return last;
		}

		PROCESS get(int i) {
			return PCS[i];
		}

		int IndexOf(PROCESS Process) {

			for(int i = 0; i < last; i++)
				if(PCS[i].Prc.th32ProcessID == Process.Prc.th32ProcessID)
					return i;
			return -1;
		}

		PROCESS Dequeue(int index) {
			PROCESS F = PCS[index];
			for(int i = index; i < last; i++)
				PCS[i] = PCS[i + 1];
			last--;
			return F;
		}

		void Enqueue(PROCESS P) {
			PCS[last++] = P;
		}

	private:
		PROCESS *PCS;
		int last;
		static const int max = 1024;
};

Queue Q1 = Queue();
Queue Q2 = Queue();
Queue Q3 = Queue();
Queue Q4 = Queue();

Queue *QS = new Queue[4];
int pauses[4] = { 50, 100, 150, 200 };

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

bool AllocTimeQueue(PROCESS Process) {

	if(Process.Prc.cntThreads != 0 && Process.QID < 3) {
		int index = QS[Process.QID].IndexOf(Process);
		if(index != -1) {
			QS[Process.QID].Dequeue(index);
			QS[Process.QID + 1].Enqueue(Process);
			return true;
		}
	}

	return false;
}

int _tmain(int argc, _TCHAR* argv[]) {

	PROCESS* Processes = new PROCESS[1024];
	int count = 0;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W entry = PROCESSENTRY32W();
	entry.dwSize = sizeof(entry);

	if (!Process32FirstW(snap, &entry))
		return 0;

	do {

		int i = count++;
		for( ; i > 0 && Processes[i - 1].Prc.pcPriClassBase > entry.pcPriClassBase; i--)
			Processes[i] = Processes[i - 1];
		Processes[i] = PROCESS(entry, 0); 
	} 
	while (Process32NextW(snap, &entry));

	/*for(int i = 0; i < count; i++)
		wprintf(_T("%s: %d\n"), Processes[i].Prc.szExeFile, Processes[i].Prc.pcPriClassBase);*/

	/*int pause = 100;
	while(true) {

		int id = AllocTime(Processes, count);
		wprintf(_T("Process %s recieved a quant of time\n"), Processes[id].Prc.szExeFile);
		Sleep(pause);
	}*/

	QS[0] = Q1;
	QS[1] = Q2;
	QS[2] = Q3;
	QS[3] = Q4;

	for(int i = 0; i < count; i++)
		QS[0].Enqueue(Processes[i]);

	while(true) {

		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < QS[i].size(); j++) {
				PROCESS p = QS[i].get(j);
				AllocTimeQueue(p);
				wprintf(_T("Process %s from queue %d recieved a quant of time\n"), p.Prc.szExeFile, i + 1);
				Sleep(pauses[i]);
			}
		}
	}

	system("pause");
	return 0;
}

