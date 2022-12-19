#include "HeapManager.h"

HeapManager* HeapManager::create(void* i_pMemory, const size_t i_sizeMemory, const unsigned int i_numDescriptors)
{
	assert((i_pMemory != nullptr) && (i_sizeMemory > sizeof(MemoryBlock)));

	HeapManager* heapManager = reinterpret_cast<HeapManager*>(i_pMemory);

	i_pMemory = static_cast<void*>(static_cast<uintptr_t*>(i_pMemory) + sizeof(HeapManager));
	
	heapManager->pFreeList = reinterpret_cast<MemoryBlock*>(i_pMemory);

	MemoryBlock* pCurrentBlock = heapManager->pFreeList;

	for (size_t i = 0; i < (static_cast<unsigned long long>(i_numDescriptors) - 1); i++, pCurrentBlock++)
	{
		pCurrentBlock->pBaseAddress = nullptr;
		pCurrentBlock->BlockSize = 0;
		pCurrentBlock->pNextBlock = pCurrentBlock + 1;
	}
	// last block, end the list
	pCurrentBlock->pBaseAddress = nullptr;
	pCurrentBlock->BlockSize = 0;
	pCurrentBlock->pNextBlock = nullptr;

	MemoryBlock* pBlock = heapManager->GetMemoryBlock();
	pBlock->pBaseAddress = static_cast<void*>(static_cast<uintptr_t*>(i_pMemory) + sizeof(MemoryBlock) * i_numDescriptors);
	pBlock->BlockSize = i_sizeMemory - sizeof(MemoryBlock) * i_numDescriptors - sizeof(HeapManager);
	pBlock->pNextBlock = heapManager->FreeList;
	heapManager->FreeList = pBlock;

	return heapManager;
}

inline MemoryBlock* HeapManager::GetMemoryBlock()
{
	assert(pFreeList != nullptr);

	MemoryBlock* pReturnBlock = pFreeList;
	pFreeList = pFreeList->pNextBlock;

	return pReturnBlock;
}

// Returning a node to pFreeBlock after coalescing
inline void HeapManager::ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
{
	assert(i_pFreeBlock != nullptr);
	i_pFreeBlock->pBaseAddress = nullptr;
	i_pFreeBlock->BlockSize = 0;
	i_pFreeBlock->pNextBlock = pFreeList;
	pFreeList = i_pFreeBlock;
}

void* HeapManager::_alloc(const size_t i_size)
{
	MemoryBlock* pBlock = GetMemoryBlock();

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
		pFreeBlock = FreeList;
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
	pFreeBlock->pBaseAddress = static_cast<uintptr_t*>(pFreeBlock->pBaseAddress) + i_size;
	pFreeBlock->pBaseAddress = static_cast<void*>(pFreeBlock->pBaseAddress);
	pFreeBlock->BlockSize -= i_size;

	return pBlock->pBaseAddress;
}

bool HeapManager::_free(const void* i_ptr)
{
	// remove the block for this pointer from OutstandingAllocations
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
				pBlock->pNextBlock = pFreeBlock;
				break;
			}
		}
	}
	return true;
}

inline void HeapManager::TrackAllocation(MemoryBlock* pBlock)
{
	pBlock->pNextBlock = OutstandingAllocations;
	OutstandingAllocations = pBlock;
}

inline bool HeapManager::Contains(const void* i_ptr)const
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

inline void HeapManager::collect()
{
	MemoryBlock* pBlock = FreeList;
	MemoryBlock* pReturnBlock;

	while (pBlock->pNextBlock)
	{
		if (static_cast<uintptr_t*>(pBlock->pBaseAddress) + pBlock->BlockSize == static_cast<uintptr_t*>(pBlock->pNextBlock->pBaseAddress))
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

bool HeapManager::IsAllocated(void* i_ptr) const
{
	if (i_ptr)
		return true;
	else
		return false;
}