#pragma once

class HeapManager {
public:
	static HeapManager* create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	void destroy();
	void* _alloc(size_t i_size);
	void* _alloc(size_t i_size, unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	bool Contains(void* i_ptr);
	bool IsAllocated(void* i_ptr);
	size_t getLargestFreeBlock();
	size_t getTotalFreeMemory();
	void ShowFreeBlocks();
};

HeapManager* HeapManager::create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{

}