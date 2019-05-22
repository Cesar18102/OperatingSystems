#include "stdafx.h"
#define Align(size, al) ((size + al - 1) / al * al) 

typedef struct { 

	PVOID Address; 
	DWORD Size; 
	DWORD State; // MEM_COMMIT, MEM_RESERVE. MEM_FREE 
} REGION; 

MEMORY_BASIC_INFORMATION MemBsInfo = MEMORY_BASIC_INFORMATION();
SYSTEM_INFO Sinfo = SYSTEM_INFO();
HANDLE h = GetCurrentProcess();

REGION* regs = new REGION[256];
int rgCount = 0;

int GetRegionsList(REGION *Regs){ 

	GetSystemInfo(&Sinfo); 

	DWORD StartAddress = (DWORD)Sinfo.lpMinimumApplicationAddress; 
	DWORD EndAddress = (DWORD)Sinfo.lpMaximumApplicationAddress; 
	DWORD _Align = Sinfo.dwAllocationGranularity; 

	int count = 0;
	for (DWORD i = StartAddress; i < EndAddress; count++){ 

		REGION Item = REGION();
		VirtualQueryEx(h, (LPVOID)i, &MemBsInfo, sizeof(MemBsInfo)); 

		Item.Address = MemBsInfo.BaseAddress; //AllocationBase
		Item.Size = Align(MemBsInfo.RegionSize, _Align); 
		Item.State = MemBsInfo.State; 

		Regs[count] = Item; 
		i += Item.Size; 
	}

	return count;
}

void* MemAlloc(int Size) {

	if(rgCount == 0)
		return NULL;

	int minInd = -1;
	for (int i = 0; i < rgCount; i++)
		if(regs[i].State == MEM_FREE && (minInd == -1 || (regs[minInd].Size > regs[i].Size && regs[i].Size >= Size)))
			minInd = i;

	if(minInd == -1)
		return NULL;
	else {

		REGION *allocReg = regs + minInd;
		void *reserved = VirtualAllocEx(h, allocReg, Size, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		return VirtualAllocEx(h, reserved, Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
}

void MemFree(void *Mem, int Size) {

	VirtualFreeEx(h, Mem, Size, MEM_DECOMMIT);
	VirtualFreeEx(h, Mem, Size, MEM_RELEASE);
}

int _tmain(int argc, _TCHAR* argv[]) {

	rgCount = GetRegionsList(regs);

	for(int i = 0; i < rgCount; i++) {

		void *mem = MemAlloc(4);
		int *n = (int*)mem;
		*n = i;

		printf("%d\n", *n);

		MemFree(mem, 4);
	}

	system("pause");
	return 0;
}

