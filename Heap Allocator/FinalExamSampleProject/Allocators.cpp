#include <inttypes.h>
#include <malloc.h>
#include "MemorySystem.h"
#include <stdio.h>

void * __cdecl malloc(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	
	printf("malloc %zu\n", i_size);

	void* pReturn = nullptr;

	pReturn = pFixedSizeAllocator->alloc(i_size);

	// if the FSA is not available for this size
	if (pReturn == nullptr)
		pReturn = pHeapManager->_alloc(i_size);

	return pReturn;
}

void __cdecl free(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	if (!pFixedSizeAllocator->free(i_ptr))
		pHeapManager->_free(i_ptr);
}

void * operator new(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new %zu\n", i_size);

	void* pReturn = nullptr;

	pReturn = pFixedSizeAllocator->alloc(i_size);

	// if the FSA is not available for this size
	if (pReturn == nullptr)
		pReturn = pHeapManager->_alloc(i_size);

	return pReturn;
}

void operator delete(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	if (!pFixedSizeAllocator->free(i_ptr))
		pHeapManager->_free(i_ptr);
}

void * operator new[](size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);

	void* pReturn = nullptr;

	pReturn = pFixedSizeAllocator->alloc(i_size);

	// if the FSA is not available for this size
	if (pReturn == nullptr)
		pReturn = pHeapManager->_alloc(i_size);

	return pReturn;
}

void operator delete [](void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));

	if (!pFixedSizeAllocator->free(i_ptr))
		pHeapManager->_free(i_ptr);
}