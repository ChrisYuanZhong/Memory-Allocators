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
	void destroy();
	void* _alloc(size_t i_size);
	void* _alloc(size_t i_size, unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	bool Contains(void* i_ptr) const;
	bool IsAllocated(void* i_ptr) const;
	size_t getLargestFreeBlock() const;
	size_t getTotalFreeMemory() const;
	void ShowFreeBlocks() const;

private:
	struct MemoryBlock TheHeap;
	struct MemoryBlock* FreeList = &TheHeap;
};

HeapManager* HeapManager::create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
}