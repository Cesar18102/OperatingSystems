#include "stdafx.h"
#include <iostream>
#include <map>

#define Align(size, al) ((size + al - 1) / al * al) 

using namespace std;

class Node {
public:
	int key, value;
	Node *prev, *next;
	Node(int k, int v) : key(k), value(v), prev(NULL), next(NULL) {}
};

class DoublyLinkedList {
	Node *front, *rear;

	bool isEmpty() {
		return rear == NULL;
	}

public:
	DoublyLinkedList() : front(NULL), rear(NULL) {}

	Node* add_page_to_head(int key, int value) {
		Node *page = new Node(key, value);
		if (!front && !rear) {
			front = rear = page;
		}
		else {
			page->next = front;
			front->prev = page;
			front = page;
		}
		return page;
	}

	void move_page_to_head(Node *page) {
		if (page == front) {
			return;
		}
		if (page == rear) {
			rear = rear->prev;
			rear->next = NULL;
		}
		else {
			page->prev->next = page->next;
			page->next->prev = page->prev;
		}

		page->next = front;
		page->prev = NULL;
		front->prev = page;
		front = page;
	}

	void remove_rear_page() {
		if (isEmpty()) {
			return;
		}
		if (front == rear) {
			delete rear;
			front = rear = NULL;
		}
		else {
			Node *temp = rear;
			rear = rear->prev;
			rear->next = NULL;
			delete temp;
		}
	}
	Node* get_rear_page() {
		return rear;
	}

};

class LRUCache {
	int capacity, size;
	DoublyLinkedList *pageList;
	map<int, Node*> pageMap;

public:
	LRUCache(int capacity) {
		this->capacity = capacity;
		size = 0;
		pageList = new DoublyLinkedList();
		pageMap = map<int, Node*>();
	}

	int get(int key) {
		if (pageMap.find(key) == pageMap.end()) {
			return -1;
		}
		int val = pageMap[key]->value;

		// move the page to front
		pageList->move_page_to_head(pageMap[key]);
		return val;
	}

	void put(int key, int value) {
		if (pageMap.find(key) != pageMap.end()) {
			// if key already present, update value and move page to head
			pageMap[key]->value = value;
			pageList->move_page_to_head(pageMap[key]);
			return;
		}

		if (size == capacity) {
			// remove rear page
			int k = pageList->get_rear_page()->key;
			pageMap.erase(k);
			pageList->remove_rear_page();
			size--;
		}

		// add new page to head to Queue
		Node *page = pageList->add_page_to_head(key, value);
		size++;
		pageMap[key] = page;
	}

	~LRUCache() {
		map<int, Node*>::iterator i1;
		for (i1 = pageMap.begin(); i1 != pageMap.end(); i1++) {
			delete i1->second;
		}
		delete pageList;
	}
};

typedef struct { 

	PVOID Address; 
	DWORD Size; 
	DWORD State; // MEM_COMMIT, MEM_RESERVE. MEM_FREE 
} REGION; 

MEMORY_BASIC_INFORMATION MemBsInfo = MEMORY_BASIC_INFORMATION();
MEMORYSTATUSEX MemoryStatus = MEMORYSTATUSEX(); //MEMORY STATE
SYSTEM_INFO SysInfo = SYSTEM_INFO(); 
HANDLE h = GetCurrentProcess();

REGION* regs = new REGION[256];
int rgCount = 0;

int GetRegionsList(REGION *Regs){ 

	GetSystemInfo(&SysInfo); 

	DWORD StartAddress = (DWORD)SysInfo.lpMinimumApplicationAddress; 
	DWORD EndAddress = (DWORD)SysInfo.lpMaximumApplicationAddress; 
	DWORD _Align = SysInfo.dwAllocationGranularity; 

	int count = 0;
	for (DWORD i = StartAddress; i < EndAddress; count++){ 

		REGION Item = REGION();
		VirtualQueryEx(h, (LPVOID)i, &MemBsInfo, sizeof(MemBsInfo)); 

		Item.Address = MemBsInfo.BaseAddress;
		Item.Size = Align(MemBsInfo.RegionSize, _Align); 
		Item.State = MemBsInfo.State; 

		Regs[count] = Item;
		i += Item.Size; 
	}

	return count;
}

void RunLRUCache() {
	LRUCache cache(2);  
	cache.put(2, 2);
	cout << cache.get(2) << endl;
	cout << cache.get(1) << endl;
	cache.put(1, 1);
	cache.put(1, 5);
	cout << cache.get(1) << endl;
	cout << cache.get(2) << endl;
	cache.put(8, 8);
	cout << cache.get(1) << endl;
	cout << cache.get(8) << endl;
}

void printRegionsList(REGION* Regs, int Count) { 

	for(int i = 0; i < Count; i++)
		_tprintf(_T("%p\t%x\t%s\n"), Regs[i].Address, Regs[i].Size, Regs[i].State == MEM_FREE? _T("FREE") : _T("BUSY")); 
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

typedef struct { 
	PVOID va, fa; 
} PAGE; 

PVOID AddPage(PVOID fa[], size_t faCount, PAGE *Pages, int PageCount, PAGE *Page) { 

	BOOL bFind = FALSE; 
	PAGE Item = PAGE(); 
	for (int i = 0; i < PageCount; i++) { 
		Item = Pages[i]; 
		if (Item.va == Page->va) { 
			Page->fa = Item.fa; 

			for(int j = PageCount - 1; j > i; j--)
				Pages[j - 1] = Pages[j];
			PageCount--;

			bFind = TRUE; 
			break; 
		} 
	}

	if (!bFind) { 
		if (PageCount == faCount) {
			Item = Pages[0];
			Page->fa = Item.fa;

			for(int j = PageCount - 1; j > 0; j--)
				Pages[j - 1] = Pages[j];
			PageCount--;
		} else
			Page->fa = fa[PageCount]; 
	} 

	Pages[PageCount++] = *Page;
	return Page->fa;
}

int GetCacheNumber(int p[4], int n) {

	static int flags; 
	int RetValue = -1; 

	for(int i = 0; i < 4; ++i) { 
		if(p[i] == n) { 
			if(i < 2) { 

				flags |=1; 

				if(i & 1) 
					flags &= ~2;
				else 
					flags |= 2; 

			} else { 

				flags &= ~1; 

				if(i & 1) 
					flags &= ~4;
				else 
					flags |= 4; 
			} 
			RetValue = i; 
		} 
	} 
	
	if(RetValue == -1) { 
		if(flags & 1) { 

			if(flags & 4) 
				RetValue = 3;
			else 
				RetValue = 2;

			flags ^=4;

		} else { 

			if(flags & 2) 
				RetValue = 1;
			else 
				RetValue = 0;

			flags ^=2;
		}

		flags ^=1; 
		p[RetValue] = n; 
	}

	printf("%d\t%d %d %d\t%d\n", n, flags & 1, (flags>> 1) &1, (flags>> 2) &1, RetValue); 
	return RetValue; 
}

int _tmain(int argc, _TCHAR* argv[]) {

	GetSystemInfo(&SysInfo); 

	printf("----------SYSTEM INFO----------\n\n");
	printf("PageSize = %d\n", SysInfo.dwPageSize); 
	printf("AllocationGranularity = %d\n", SysInfo.dwAllocationGranularity); 
	printf("MinimumApplicationAddress = %#x\n", SysInfo.lpMinimumApplicationAddress); 
	printf("MaximumApplicationAddress = %#x\n", SysInfo.lpMaximumApplicationAddress);
	printf("\n//////////SYSTEM INFO//////////\n\n");

	MemoryStatus.dwLength = sizeof(MEMORYSTATUSEX); 
	GlobalMemoryStatusEx(&MemoryStatus); 
	
	printf("----------MEMORY STATE----------\n\n");
	printf("ullAvailPhys = %I64x\n", MemoryStatus.ullAvailPhys); 
	printf("ullAvailVirtual = %I64x\n", MemoryStatus.ullAvailVirtual); 
	printf("ullAvailPageFile = %I64x\n", MemoryStatus.ullAvailPageFile); 
	printf("ullullTotalPhys = %I64x\n", MemoryStatus.ullTotalPhys); 
	printf("ullTotalVirtual = %I64x\n", MemoryStatus.ullTotalVirtual); 
	printf("ullTotalPageFile = %I64x\n", MemoryStatus.ullTotalPageFile);
	printf("\n//////////MEMORY STATE//////////\n\n");

	
	DWORD Address = (DWORD)SysInfo.lpMinimumApplicationAddress; 
	
	printf("----------MEMORY INFO----------\n\n");
	while(Address < (DWORD)SysInfo.lpMaximumApplicationAddress) { 
		
		DWORD dwSize = VirtualQueryEx(h, (LPVOID)Address, &MemBsInfo, sizeof(MEMORY_BASIC_INFORMATION)); 
		printf("BaseAddress = %#x\n", Address); 
		printf("RegionSize = %u\n", MemBsInfo.RegionSize); 
		
		switch(MemBsInfo.State) { 
			case MEM_COMMIT : printf("MEM_COMMIT\n"); break; 
			case MEM_FREE : printf("MEM_FREE\n"); break; 
			case MEM_RESERVE : printf("MEM_RESERVE\n"); break; 
			default : printf("UNKNOWN\n"); 
		} 
		
		if(MemBsInfo.State != MEM_FREE) { 
			switch (MemBsInfo.AllocationProtect) { 
				case PAGE_READONLY : printf("PAGE_READONLY\n"); break; 
				case PAGE_READWRITE : printf("PAGE_READWRITE\n"); break; 
				case PAGE_EXECUTE : printf("PAGE_READWRITE\n"); break; 
				case PAGE_EXECUTE_READ : printf("PAGE_EXECUTE_READ\n"); break; 
				case PAGE_EXECUTE_READWRITE : printf("PAGE_EXECUTE_READWRITE\n"); break; 
				case PAGE_EXECUTE_WRITECOPY : printf("PAGE_EXECUTE_WRITECOPY\n"); break; 
				case PAGE_NOACCESS : printf("PAGE_NOACCESS\n"); break; 
				case PAGE_WRITECOPY : printf("PAGE_WRITECOPY\n"); break; 
				default : printf("UNKNOWN\n"); 
			} 
		}

		printf("\n=====================================================\n\n"); 
		Address += MemBsInfo.RegionSize;
	}
	printf("\n//////////MEMORY INFO//////////\n\n");

	printf("----------MEMORY REGIONS----------\n\n");

	rgCount = GetRegionsList(regs);
	printRegionsList(regs, rgCount);

	printf("\n//////////MEMORY REGIONS//////////\n\n");

	printf("----------LRU CACHE----------\n\n");

	RunLRUCache();

	printf("\n//////////LRU CACHE//////////\n\n");

	system("pause");
	return 0;
}

