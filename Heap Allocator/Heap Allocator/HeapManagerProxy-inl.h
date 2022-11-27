#pragma once
#include <assert.h>
#include <stdio.h>

// This is your include file for your heap manager. It should have a class definition for HeapManager.
// If you use another name for the class, header or functions just replace what needs to be in here.
#include "HeapManager.h"

namespace HeapManagerProxy
{

	inline HeapManager* CreateHeapManager(void* i_pMemory, const size_t i_sizeMemory, const unsigned int i_numDescriptors)
	{
		return HeapManager::create(i_pMemory, i_sizeMemory, i_numDescriptors);
	}

	/*void Destroy(HeapManager* i_pManager)
	{
		assert(i_pManager);

		i_pManager->destroy();
	}*/

	inline void* alloc(HeapManager* i_pManager, const size_t i_size)
	{
		assert(i_pManager);
		return i_pManager->_alloc(i_size);
	}

	/*void* alloc(HeapManager* i_pManager, size_t i_size, unsigned int i_alignment)
	{
		assert(i_pManager);

		return i_pManager->_alloc(i_size, i_alignment);
	}*/

	inline bool free(HeapManager* i_pManager, const void* i_ptr)
	{
		assert(i_pManager);

		return i_pManager->_free(i_ptr);
	}

	inline void Collect(HeapManager* i_pManager)
	{
		assert(i_pManager);

		i_pManager->collect();
	}


	inline bool Contains(const HeapManager* i_pManager, const void* i_ptr)
	{
		assert(i_pManager);

		return i_pManager->Contains(i_ptr);
	}

	inline bool IsAllocated(const HeapManager* i_pManager, void* i_ptr)
	{
		assert(i_pManager);

		return i_pManager->IsAllocated(i_ptr);
	}

	inline void Destroy(const HeapManager* i_pManager)
	{
		assert(i_pManager);

		i_pManager->destroy();
	}

	/*size_t GetLargestFreeBlock(const HeapManager* i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getLargestFreeBlock();
	}

	size_t GetTotalFreeMemory(const HeapManager* i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getTotalFreeMemory();
	}

	void ShowFreeBlocks(const HeapManager* i_pManager)
	{
		assert(i_pManager);

		i_pManager->ShowFreeBlocks();
	}

	void ShowOutstandingAllocations(const HeapManager* i_pManager)
	{
		assert(i_pManager);

#ifdef __TRACK_ALLOCATIONS
		i_pManager->ShowOutstandingAllocations();
#else
		printf("HeapManager compiled without __TRACK_ALLOCATIONS defined.\n");
#endif

	}*/

} // namespace HeapManagerProxy