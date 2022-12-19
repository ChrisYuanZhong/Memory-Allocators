#pragma once
#include "BitArray.h"
#include <windows.h>

// define the number of tiers here
constexpr int NumTiers = 3;

// define the size of each tier here
constexpr size_t Tiers[NumTiers] = { 16, 32, 96 };

// define the number of blocks of each tier here
constexpr size_t NumTierBlocks[NumTiers] = { 100, 200, 400 };

class FixedSizeAllocator {
public:
	static FixedSizeAllocator* Create(HeapManager* i_pAllocator);
	inline ~FixedSizeAllocator();
	void* alloc(const size_t i_size);
	bool free(void* i_ptr);

private:
	BitArray* bitArrays[NumTiers] = { nullptr };
	void* tierBaseAddress[NumTiers] = { nullptr };
};

inline FixedSizeAllocator::~FixedSizeAllocator()
{
#ifdef _DEBUG
	int tierIndex = 0;
	size_t bitNumber = 0;

	for (tierIndex = 0; tierIndex < NumTiers; tierIndex++)
	{
		if (bitArrays[tierIndex]->GetFirstSetBit(bitNumber))
			OutputDebugStringA("There are still outstanding allocations!");
	}
#endif
}