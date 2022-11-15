#include "HeapManagerProxy.h"
#include <Windows.h>
#include <iostream>
#include <string.h>

#include <assert.h>
#include <algorithm>
#include <vector>

//#define SUPPORTS_ALIGNMENT
//#define SUPPORTS_SHOWFREEBLOCKS
//#define SUPPORTS_SHOWOUTSTANDINGALLOCATIONS

int main()
{
	using namespace HeapManagerProxy;

	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;

#ifdef USE_HEAP_ALLOC
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
#else
	// Get SYSTEM_INFO, which includes the memory page size
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	// round our size to a multiple of memory page size
	assert(SysInfo.dwPageSize > 0);
	size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((sizeHeap + SysInfo.dwPageSize) / SysInfo.dwPageSize);
	void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#endif
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	HeapManager* pHeapManager = CreateHeapManager(pHeapMemory, sizeHeap, numDescriptors);
	assert(pHeapManager);

	if (pHeapManager == nullptr)
		return false;

	char *str = (char*)alloc(pHeapManager, 7 * sizeof(char));
	char *str1 = (char*)alloc(pHeapManager, 3 * sizeof(char));
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = 'c';
	str[4] = 'c';
	str[5] = 'c';
	str[6] = '\0';

	str1[0] = 'b';
	str1[1] = 'g';
	str1[2] = '\0';

	printf("String = %s  String1 = %s\n", str, str1);
	printf("String = %s  String1 = %s\n", str, str1);

	free(pHeapManager, str);
	printf("String = %s  String1 = %s\n", str, str1);
}