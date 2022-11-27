#pragma once
#include <stdint.h>
#include "HeapManagerProxy-inl.h"
class HeapManager;
namespace HeapManagerProxy
{
	inline HeapManager* CreateHeapManager(void* i_pMemory, const size_t i_sizeMemory,
		const unsigned int i_numDescriptors);
	inline void Destroy(const HeapManager* i_pManager);
	inline void* alloc(HeapManager* i_pManager, const size_t i_size);
	//void* alloc(HeapManager* i_pManager, size_t i_size, unsigned
	//	int i_alignment);
	inline bool free(HeapManager* i_pManager, const void* i_ptr);
	inline void Collect(HeapManager* i_pManager);
	inline bool Contains(const HeapManager* i_pManager, const void* i_ptr);
	inline bool IsAllocated(const HeapManager* i_pManager, void* i_ptr);
	//size_t GetLargestFreeBlock(const HeapManager* i_pManager);
	//size_t GetTotalFreeMemory(const HeapManager* i_pManager);
	//void ShowFreeBlocks(const HeapManager* i_pManager);
	//void ShowOutstandingAllocations(const HeapManager* i_pManager);
} // namespace HeapManagerProxy
