#pragma once
#include "FixedSizeAllocators.h"

extern HeapManager* pHeapManager = nullptr;
extern FixedSizeAllocator* pFixedSizeAllocator = nullptr;

// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);

// Collect - coalesce free blocks in attempt to create larger blocks
void Collect();

// DestroyMemorySystem - destroy your memory systems
void DestroyMemorySystem();
