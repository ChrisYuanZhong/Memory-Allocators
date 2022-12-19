1. I made the Fixed Size Allocator be able to accommodate any number of memory block size tiers, any size of each tier, and any number of blocks of each tier.
	Heap Allocator\FinalExamSampleProject\FixedSizeAllocators.h, line 5-12, line 22-23.
	Heap Allocator\FinalExamSampleProject\FixedSizeAllocators.cpp, line 9-27, line 51-66, line 80-95.

2. Implemented outputting debug message "There are still outstanding allocations!".
	Heap Allocator\FinalExamSampleProject\FixedSizeAllocators.cpp, line 32-44.

3. Implemented accommodations to both 32 and 64 bit systems.
	Heap Allocator\FinalExamSampleProject\BitArray.h, line 9-15.

4. The malloc() function will first try to allocate the memory using the FSA. The FSA will first try to allocate with the smallest tier that fits the size the user wants. If the tier is full, then try the next tier, repeatedly. And if there is no tier that fits the size the user wants, or if all the blocks that fits are unavailable, return NULL. Then, the malloc() function will attempt to allocate with the Heap Manager.
	Heap Allocator\FinalExamSampleProject\Allocators.cpp, line 14-18.
	Heap Allocator\FinalExamSampleProject\FixedSizeAllocators.cpp, line 48-72.

5. Reused IsBitSet() for IsBitClear()
	Heap Allocator\FinalExamSampleProject\BitArray.h, line 74.

6. Utilized _BitScanForward and _BitScanForward64
	Heap Allocator\FinalExamSampleProject\BitArray.h, line 11, 14, 112, and 129.
