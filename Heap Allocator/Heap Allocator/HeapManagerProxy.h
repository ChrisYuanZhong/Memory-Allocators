#pragma once
#include <stdint.h>
class HeapManager;
namespace HeapManagerProxy
{
	HeapManager* CreateHeapManager(void* i_pMemory, size_t i_sizeMemory,
		unsigned int i_numDescriptors);
	//void Destroy(HeapManager* i_pManager);
	void* alloc(HeapManager* i_pManager, size_t i_size);
	//void* alloc(HeapManager* i_pManager, size_t i_size, unsigned
	//	int i_alignment);
	bool free(HeapManager* i_pManager, void* i_ptr);
	//void Collect(HeapManager* i_pManager);
	//bool Contains(const HeapManager* i_pManager, void* i_ptr);
	//bool IsAllocated(const HeapManager* i_pManager, void* i_ptr);
	//size_t GetLargestFreeBlock(const HeapManager* i_pManager);
	//size_t GetTotalFreeMemory(const HeapManager* i_pManager);
	//void ShowFreeBlocks(const HeapManager* i_pManager);
	//void ShowOutstandingAllocations(const HeapManager* i_pManager);
} // namespace HeapManagerProxy

/*#include <assert.h>

struct MemoryBlock
{
	void* pBaseAddress;
	size_t BlockSize;
	struct MemoryBlock* pNextBlock;
};

struct MemoryBlock TheHeap;
struct MemoryBlock* FreeList = TheHeap;


void* malloc(size_t i_size)
{
	MemoryBlock* pBlock = GetFreeMemoryBlock();

	MemoryBlock* pFreeBlock = FreeList;
	while (pFreeBlock)
	{
		if (pFreeBlock->BlockSize > i_size)
			break;
		pFreeBlock = pFreeBlock->pNextBlock;
	}
	// oh no. we didn't find a block big enough
	assert(pFreeBlock);

	pBlock->pBaseAddress = pFreeBlock->pBaseAddress;
	pBlock->BlockSize = i_size;
	TrackAllocation(pBlock);

	// shrink this block
	pFreeBlock.pBaseAddress += i_size;
	pFreeBlock.BlockSize -= i_size;

	return pBlock->pBaseAddress;
}

bool free(void* i_ptr, void* ptr)
{
	// remove the block for this pointer from OutstandingAllocations
	MemoryBlock* pBlock = RemoveOutstandingAllocation(i_ptr);
	assert(pBlock);

	// put the block on the Freelist
	pBlock->pNextBlock = FreeList;
	FreeList = pBlock;
}

bool Contains(void* i_ptr, void* ptr)
{

}

void Collect(void* i_ptr)
{

}

bool IsAllocated(void* i_ptr)
{
	return true;
}*/