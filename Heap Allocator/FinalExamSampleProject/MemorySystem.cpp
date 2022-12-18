#include "MemorySystem.h"

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	// create your HeapManager and FixedSizeAllocators
	pHeapManager = HeapManager::create(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	pFixedSizeAllocator = FixedSizeAllocator::Create(pHeapManager);
	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
	pHeapManager->collect();
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
}

