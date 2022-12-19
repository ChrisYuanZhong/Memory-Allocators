#include <Windows.h>

#include "MemorySystem.h"

#include <assert.h>
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

bool MemorySystem_UnitTest();

int main()
{
	const size_t 		sizeHeap = 1024 * 1024;

	// you may not need this if you don't use a descriptor pool
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
	assert(pHeapMemory);

	// Create your HeapManager and FixedSizeAllocators.
	InitializeMemorySystem(pHeapMemory, sizeHeap, numDescriptors);

	/*bool success = MemorySystem_UnitTest();
	assert(success);

	// Clean up your Memory System (HeapManager and FixedSizeAllocators)
	DestroyMemorySystem();

	HeapFree(GetProcessHeap(), 0, pHeapMemory);

	// in a Debug build make sure we didn't leak any memory.
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;*/

	char *str = (char*)malloc(3 * sizeof(char));
	char *str1 = (char*)malloc(3 * sizeof(char));
	char* str2 = (char*)malloc(4 * sizeof(char));
	str[0] = 'a';
	str[1] = 'b';
	str[2] = '\0';
	//str[3] = 'c';
	//str[4] = 'c';
	//str[5] = 'c';
	//str[6] = '\0';

	str1[0] = 'b';
	str1[1] = 'g';
	str1[2] = '\0';

	str2[0] = 'x';
	str2[1] = 'y';
	str2[2] = 'v';
	str2[3] = '\0';

	printf("String = %s  String1 = %s  String2 = %s\n", str, str1, str2);

	free(str1);
	free(str2);
	printf("String = %s  String1 = %s  String2 = %s\n", str, str1, str2);

}