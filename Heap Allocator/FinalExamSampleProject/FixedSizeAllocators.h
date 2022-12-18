#pragma once
#include "BitArray.h"

// define the number of tiers here
constexpr int NumTiers = 3;

// define the size of each tier here
constexpr size_t Tiers[NumTiers] = { 16, 32, 96 };

// define the number of blocks of each tier here
constexpr size_t NumTierBlocks[NumTiers] = { 100, 200, 400 };

class FixedSizeAllocator {
public:
	FixedSizeAllocator(HeapManager* i_pAllocator);
	inline static FixedSizeAllocator* Create(HeapManager* i_pAllocator);
	inline ~FixedSizeAllocator();
	void* alloc(const size_t i_size);
	bool free(void* i_ptr);

private:
	BitArray* bitArrays[NumTiers] = { nullptr };
	void* tierBaseAddress[NumTiers] = { nullptr };
};

FixedSizeAllocator::FixedSizeAllocator(HeapManager* i_pAllocator)
{
	int tierIndex = 0;
	size_t totalMemory = 0;

	// create bit arrays and calculate the total size of the memory for the FSA
	for (tierIndex = 0; tierIndex < NumTiers; tierIndex++)
	{
		bitArrays[tierIndex] = BitArray::Create(NumTierBlocks[tierIndex], i_pAllocator, 1);
		totalMemory += Tiers[tierIndex] * NumTierBlocks[tierIndex];
	}
	
	// allocate and record the address of the memory for the FSA
	tierBaseAddress[0] = i_pAllocator->_alloc(totalMemory);
	if (NumTiers > 1)
	{
		for (tierIndex = 1; tierIndex < NumTiers; tierIndex++)
		{
			tierBaseAddress[tierIndex] = static_cast<void*>(static_cast<uintptr_t*>(tierBaseAddress[tierIndex - 1]) + Tiers[tierIndex - 1] * NumTierBlocks[tierIndex - 1]);
		}
	}
}

inline FixedSizeAllocator* FixedSizeAllocator::Create(HeapManager* i_pAllocator)
{
	assert(i_pAllocator && NumTiers > 0);

	FixedSizeAllocator* fixedSizeAllocator = new ((FixedSizeAllocator*)i_pAllocator->_alloc(sizeof(FixedSizeAllocator))) FixedSizeAllocator(i_pAllocator);

	return fixedSizeAllocator;
}

inline FixedSizeAllocator::~FixedSizeAllocator()
{
	int tierIndex = 0;
	size_t bitNumber = 0;

	for (tierIndex = 0; tierIndex < NumTiers; tierIndex++)
	{
		//if (bitArrays[tierIndex]->GetFirstSetBit(bitNumber))
			//OutputDebugStringA("w");
	}
}

void* FixedSizeAllocator::alloc(const size_t i_size)
{
	int tierIndex = 0;
	size_t bitNumber = 0;

	// find a tier that fits the size the user wants
	for (tierIndex = 0; tierIndex < NumTiers && i_size > Tiers[tierIndex]; tierIndex++);

	// check if that tier have an available block. If not, check the higher tier
	for (; tierIndex < NumTiers; tierIndex++)
	{
		if (!bitArrays[tierIndex]->GetFirstClearBit(bitNumber))
			continue;
		break;
	}

	// if there is no tier that fits the size the user wants, or if all the blocks that fits are unavailable, return NULL
	if (tierIndex >= NumTiers)
	{
		return NULL;
	}

	// mark it in use because we are going to allocate it to user
	bitArrays[tierIndex]->SetBit(bitNumber);

	// calculate its address and return it to user
	return static_cast<void*>(static_cast<uintptr_t*>(tierBaseAddress[tierIndex]) + (bitNumber * Tiers[tierIndex]));
}

bool FixedSizeAllocator::free(void* i_ptr)
{
	int tierIndex = 0;
	size_t bitNumber = 0;

	// find which tier does this address belong to
	for (tierIndex = 0; i_ptr > tierBaseAddress[tierIndex] && tierIndex < NumTiers; tierIndex++)
	{
		// if this address belongs to this tier
		if (i_ptr < static_cast<void*>(static_cast<uintptr_t*>(tierBaseAddress[tierIndex]) + Tiers[tierIndex] * NumTierBlocks[tierIndex]))
			break;
	}

	// if this address does not belong to any of the tiers (does not belong to the FSA)
	if (tierIndex >= NumTiers)
	{
		return false;
	}

	// find the bitNumber of this address
	bitNumber = bitArrays[tierIndex]->bitsPerElement * static_cast<size_t>(static_cast<uintptr_t*>(i_ptr) - static_cast<uintptr_t*>(tierBaseAddress[tierIndex]));

	// check if the address is an outstanding allocation
	assert(bitArrays[tierIndex]->IsBitSet(bitNumber));

	// clear the bit
	bitArrays[tierIndex]->ClearBit(bitNumber);

	return true;
}