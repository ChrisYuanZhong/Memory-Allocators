#pragma once

struct MemoryBlock
{
	void* pBaseAddress;
	size_t BlockSize;
	struct MemoryBlock* pNextBlock;
};

struct MemoryBlock* pFreeList = nullptr;

class HeapManager {
public:
	static HeapManager* create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	void destroy() const {};
	void* _alloc(size_t i_size);
	//void* _alloc(size_t i_size, unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	bool Contains(void* i_ptr) const;
	bool IsAllocated(void* i_ptr) const { return true; }
	//size_t getLargestFreeBlock() const;
	//size_t getTotalFreeMemory() const;
	//void ShowFreeBlocks() const;

	void TrackAllocation(MemoryBlock* pBlock);

//private:
	struct MemoryBlock* FreeList = nullptr;
	struct MemoryBlock* OutstandingAllocations = nullptr;
};

MemoryBlock* GetMemoryBlock()
{
	assert(pFreeList != nullptr);

	MemoryBlock* pReturnBlock = pFreeList;
	pFreeList = pFreeList->pNextBlock;

	return pReturnBlock;
}

MemoryBlock* GetFreeMemoryBlock()
{
	return GetMemoryBlock();
}

// Returning a node to pFreeBlock after coalescing
void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
{
	assert(i_pFreeBlock != nullptr);
	i_pFreeBlock->pBaseAddress = nullptr;
	i_pFreeBlock->BlockSize = 0;
	i_pFreeBlock->pNextBlock = pFreeList;
	pFreeList = i_pFreeBlock;
}

HeapManager* HeapManager::create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
	assert((i_pMemory != nullptr) && (i_sizeMemory > sizeof(MemoryBlock)));

	static HeapManager heapManager;

	pFreeList = reinterpret_cast<MemoryBlock*>(i_pMemory);

	/*heapManager.pFreeList->pBaseAddress = heapManager.pFreeList + sizeof(MemoryBlock);
	heapManager.pFreeList->BlockSize = i_sizeMemory - sizeof(MemoryBlock);
	heapManager.pFreeList->pNextBlock = nullptr;*/

	const size_t NumberofBlocks = i_sizeMemory / sizeof(MemoryBlock);

	MemoryBlock* pCurrentBlock = pFreeList;

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

	MemoryBlock* pBlock = GetMemoryBlock();
	pBlock->pBaseAddress = static_cast<char*>(pBlock->pBaseAddress);
	pBlock->pBaseAddress = static_cast<char*>(i_pMemory) + sizeof(pFreeList) + sizeof(heapManager.FreeList) + sizeof(heapManager) + 16;
	pBlock->pBaseAddress = static_cast<void*>(pBlock->pBaseAddress);
	pBlock->BlockSize = i_sizeMemory - sizeof(pFreeList) - sizeof(heapManager.FreeList) - sizeof(heapManager);
	pBlock->pNextBlock = heapManager.FreeList;
	heapManager.FreeList = pBlock;
	
	//pFreeList->BlockSize = i_sizeMemory;

	return &heapManager;
}

void* HeapManager::_alloc(size_t i_size)
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
	if (!pFreeBlock)
	{
		collect();
		MemoryBlock* pFreeBlock = FreeList;
		while (pFreeBlock)
		{
			if (pFreeBlock->BlockSize > i_size)
				break;
			pFreeBlock = pFreeBlock->pNextBlock;
		}
	}

	// oh no. we didn't find a block big enough
	if (!pFreeBlock)
		return nullptr;

	pBlock->pBaseAddress = pFreeBlock->pBaseAddress;
	pBlock->BlockSize = i_size;
	TrackAllocation(pBlock);

	//shrink this block
	pFreeBlock->pBaseAddress = static_cast<char*>(pFreeBlock->pBaseAddress) + i_size;
	//pFreeBlock->pBaseAddress += i_size;
	pFreeBlock->pBaseAddress = static_cast<void*>(pFreeBlock->pBaseAddress);
	pFreeBlock->BlockSize -= i_size;

	return pBlock->pBaseAddress;
}

bool HeapManager::_free(void* i_ptr)
{
	// remove the block for this pointer from OutstandingAllocations
	//MemoryBlock* pBlock = (MemoryBlock*)i_ptr - sizeof(MemoryBlock);
	MemoryBlock* pBlock = OutstandingAllocations;
	MemoryBlock* pFreeBlock = nullptr;
	
	if (pBlock->pBaseAddress == i_ptr)
	{
		pFreeBlock = pBlock;
		OutstandingAllocations = OutstandingAllocations->pNextBlock;
	}
	else
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
		if (!pFreeBlock)
			return false;
		pBlock->pNextBlock = pFreeBlock->pNextBlock;
	}

	// put the block on the Freelist
	assert(pFreeBlock);
	if (!FreeList || pFreeBlock->pBaseAddress < FreeList->pBaseAddress)
	{
		pFreeBlock->pNextBlock = FreeList;
		FreeList = pFreeBlock;
	}
	else
	{
		for (pBlock = FreeList; pBlock->pNextBlock; pBlock = pBlock->pNextBlock)
		{
			if (pFreeBlock->pBaseAddress < pBlock->pNextBlock->pBaseAddress)
			{
				pFreeBlock->pNextBlock = pBlock->pNextBlock;
				//printf("String = %s  String1 = %s\n", OutstandingAllocations->pBaseAddress, pFreeBlock->pBaseAddress);
				pBlock->pNextBlock = pFreeBlock;
				break;
			}
		}
	}
	return true;
}

void HeapManager::TrackAllocation(MemoryBlock* pBlock)
{
	pBlock->pNextBlock = OutstandingAllocations;
	OutstandingAllocations = pBlock;
}

bool HeapManager::Contains(void* i_ptr)const
{
	MemoryBlock* pBlock = OutstandingAllocations;

	while (pBlock)
	{
		if (pBlock->pBaseAddress == i_ptr)
			return true;
		pBlock = pBlock->pNextBlock;
	}

	return false;
}

void HeapManager::collect()
{
	MemoryBlock* pBlock = FreeList;
	MemoryBlock* pReturnBlock;

	while (pBlock->pNextBlock)
	{
		if (static_cast<char*>(pBlock->pBaseAddress) + pBlock->BlockSize == static_cast<char*>(pBlock->pNextBlock->pBaseAddress))
		{
			// expand block 1 to include block 2 memory
			pBlock->BlockSize += pBlock->pNextBlock->BlockSize;

			pReturnBlock = pBlock->pNextBlock;

			pBlock->pNextBlock = pBlock->pNextBlock->pNextBlock;

			// put pBlock2's MemoryBlock in the free pool (of MemoryBlocks)
			ReturnMemoryBlock(pReturnBlock);

			continue;
		}
		pBlock = pBlock->pNextBlock;
	}
}