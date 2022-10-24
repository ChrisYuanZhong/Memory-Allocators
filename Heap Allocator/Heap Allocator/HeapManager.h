#pragma once

struct MemoryBlock
{
	void* pBaseAddress;
	size_t BlockSize;
	struct MemoryBlock* pNextBlock;
};

class HeapManager {
public:
	static HeapManager* create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	//void destroy();
	void* _alloc(size_t i_size);
	//void* _alloc(size_t i_size, unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	bool Contains(void* i_ptr) const;
	//bool IsAllocated(void* i_ptr) const;
	//size_t getLargestFreeBlock() const;
	//size_t getTotalFreeMemory() const;
	//void ShowFreeBlocks() const;

private:
	struct MemoryBlock* pFreeList = nullptr;
	struct MemoryBlock* OutstandingAllocations = nullptr;
};

HeapManager heapManager;

HeapManager* HeapManager::create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
	assert((i_pMemory != nullptr) && (i_sizeMemory > sizeof(MemoryBlock)));

	heapManager.pFreeList = reinterpret_cast<MemoryBlock*>(i_pMemory);

	heapManager.pFreeList->pBaseAddress = heapManager.pFreeList + sizeof(MemoryBlock);
	heapManager.pFreeList->BlockSize = i_sizeMemory - sizeof(MemoryBlock);
	heapManager.pFreeList->pNextBlock = nullptr;

	return &heapManager;

	/*const size_t NumberofBlocks = i_sizeMemory / sizeof(MemoryBlock);

	MemoryBlock* pCurrentBlock = heapManager.pFreeList;

	for (size_t i = 0; i < (NumberofBlocks - 1); i++, pCurrentBlock++)
	{
		pCurrentBlock->pBaseAddress = nullptr;
		pCurrentBlock->BlockSize = 0;
		pCurrentBlock->pNextBlock = pCurrentBlock + 1;
	}
	// last block, end the list
	pCurrentBlock->pBaseAddress = nullptr;
	pCurrentBlock->BlockSize = 0;
	pCurrentBlock->pNextBlock = nullptr;

	heapManager.pFreeList->BlockSize = i_sizeMemory;*/

}

void* HeapManager::_alloc(size_t i_size)
{
	MemoryBlock* pFreeBlock = pFreeList;
	MemoryBlock* pBlock = pFreeBlock;

	if (pFreeBlock->BlockSize < i_size)
	{
		while (pFreeBlock->pNextBlock)
		{
			if (pFreeBlock->pNextBlock->BlockSize > i_size)
			{
				pBlock = pFreeBlock->pNextBlock;
				break;
			}
			pFreeBlock = pFreeBlock->pNextBlock;
		}
		// oh no. we didn't find a block big enough
		assert(pBlock);

		// create a new node to represent the rest of this block
		MemoryBlock* pNewNode = (MemoryBlock*)pBlock->pBaseAddress + i_size;
		pNewNode->pBaseAddress = pNewNode + sizeof(MemoryBlock);
		pNewNode->BlockSize = pBlock->BlockSize - i_size - sizeof(MemoryBlock);
		pNewNode->pNextBlock = pBlock->pNextBlock;
		pFreeBlock->pNextBlock = pNewNode;
	}
	else
	{
		assert(pBlock);

		// create a new node to represent the rest of this block
		MemoryBlock* pNewNode = (MemoryBlock*)pBlock->pBaseAddress + i_size;
		pNewNode->pBaseAddress = pNewNode + sizeof(MemoryBlock);
		pNewNode->BlockSize = pBlock->BlockSize - i_size - sizeof(MemoryBlock);
		pNewNode->pNextBlock = pBlock->pNextBlock;
		pFreeList = pNewNode;
	}
	
	pBlock->BlockSize = i_size;
	// track allocation
	pBlock->pNextBlock = OutstandingAllocations;
	OutstandingAllocations = pBlock;

	return pBlock->pBaseAddress;
}

bool HeapManager::_free(void* i_ptr)
{
	// remove the block for this pointer from OutstandingAllocations
	//MemoryBlock* pBlock = (MemoryBlock*)i_ptr - sizeof(MemoryBlock);
	MemoryBlock* pBlock = OutstandingAllocations;
	MemoryBlock* pFreeBlock = pBlock;
	if (pBlock->pBaseAddress != i_ptr)
	{
		while (pBlock->pNextBlock)
		{
			if (pBlock->pNextBlock->pBaseAddress == i_ptr)
			{
				pFreeBlock = pBlock->pNextBlock;
				break;
			}
			pBlock = pBlock->pNextBlock;
		}
		pBlock->pNextBlock = pFreeBlock->pNextBlock;
	}
	assert(pFreeBlock);


	// put the block on the Freelist
	pFreeBlock->pNextBlock = pFreeList;
	pFreeList = pFreeBlock;

	return true;
}