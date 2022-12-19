#pragma once
#include <cassert>

struct MemoryBlock
{
	void* pBaseAddress;
	size_t BlockSize;
	struct MemoryBlock* pNextBlock;
};

class HeapManager {
public:
	static HeapManager* create(void* i_pMemory, const size_t i_sizeMemory, const unsigned int i_numDescriptors);
	inline MemoryBlock* GetMemoryBlock();
	inline void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock);
	void* _alloc(const size_t i_size);
	bool _free(const void* i_ptr);
	inline void collect();
	inline bool Contains(const void* i_ptr) const;
	inline bool IsAllocated(void* i_ptr) const;
	inline void TrackAllocation(MemoryBlock* pBlock);

private:
	struct MemoryBlock* pFreeList = nullptr;
	struct MemoryBlock* FreeList = nullptr;
	struct MemoryBlock* OutstandingAllocations = nullptr;
};

