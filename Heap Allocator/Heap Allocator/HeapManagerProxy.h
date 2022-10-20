#include <assert.h>

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
}