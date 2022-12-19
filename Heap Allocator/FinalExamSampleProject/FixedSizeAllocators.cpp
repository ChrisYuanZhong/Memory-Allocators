#include "FixedSizeAllocators.h"

FixedSizeAllocator* FixedSizeAllocator::Create(HeapManager* i_pAllocator)
{
	assert(i_pAllocator && NumTiers > 0);

	FixedSizeAllocator* fixedSizeAllocator = reinterpret_cast<FixedSizeAllocator*>(i_pAllocator->_alloc(sizeof(FixedSizeAllocator)));

	int tierIndex = 0;
	size_t totalMemory = 0;

	// create bit arrays and calculate the total size of the memory for the FSA
	for (tierIndex = 0; tierIndex < NumTiers; tierIndex++)
	{
		fixedSizeAllocator->bitArrays[tierIndex] = BitArray::Create(NumTierBlocks[tierIndex], i_pAllocator, 1);
		totalMemory += Tiers[tierIndex] * NumTierBlocks[tierIndex];
	}

	// allocate and record the address of the memory for the FSA
	fixedSizeAllocator->tierBaseAddress[0] = i_pAllocator->_alloc(totalMemory);
	if (NumTiers > 1)
	{
		for (tierIndex = 1; tierIndex < NumTiers; tierIndex++)
		{
			fixedSizeAllocator->tierBaseAddress[tierIndex] = static_cast<void*>(static_cast<uintptr_t*>(fixedSizeAllocator->tierBaseAddress[tierIndex - 1]) + Tiers[tierIndex - 1] * NumTierBlocks[tierIndex - 1]);
		}
	}

	return fixedSizeAllocator;
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
	bitNumber = static_cast<size_t>(static_cast<uintptr_t*>(i_ptr) - static_cast<uintptr_t*>(tierBaseAddress[tierIndex])) / Tiers[tierIndex];

	// check if the address is an outstanding allocation
	assert(bitArrays[tierIndex]->IsBitSet(bitNumber));

	// clear the bit
	bitArrays[tierIndex]->ClearBit(bitNumber);

	return true;
}