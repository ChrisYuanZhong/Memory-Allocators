#pragma once
#include <iostream>
#include <cstdint>
#include <vcruntime_string.h>
#include "HeapManager.h"
#include <intrin.h>

class BitArray
{
#ifdef WIN32
	typedef uint32_t t_BitData;
#define BitScanForward _BitScanForward
#else
	typedef uint64_t t_BitData;
#define BitScanForward _BitScanForward64
#endif?//?WIN32

public:
	inline BitArray(const size_t i_numBits, const bool i_bInitToZero);
	inline static BitArray* Create(const size_t i_numBits, HeapManager* i_pAllocator, const bool i_startClear = true);

	inline ~BitArray();


	inline bool IsBitSet(const size_t i_bitNumber) const;
	inline bool IsBitClear(const size_t i_bitNumber) const;

	inline void SetBit(const size_t i_bitNumber);
	inline void ClearBit(const size_t i_bitNumber);

	inline void SetAll();
	inline void ClearAll();

	inline bool GetFirstClearBit(size_t& o_bitNumber) const;
	inline bool GetFirstSetBit(size_t& o_bitNumber) const;

	bool operator[](size_t i_index) const;

	const size_t bitsPerElement = sizeof(t_BitData) * 8;
private:
	size_t numBits = 0;
	t_BitData* m_pBits = nullptr;
};

inline BitArray::BitArray(const size_t i_numBits, const bool i_bInitToZero)
{
	numBits = i_numBits;
	m_pBits = new t_BitData[i_numBits % bitsPerElement == 0 && i_numBits / bitsPerElement != 0 ? i_numBits / bitsPerElement : (i_numBits / bitsPerElement) + 1];
	memset(m_pBits, i_bInitToZero ? 0 : -1, (i_numBits % bitsPerElement == 0 && numBits / bitsPerElement != 0 ? i_numBits / bitsPerElement : (i_numBits / bitsPerElement) + 1) * sizeof(t_BitData));
}

inline BitArray* BitArray::Create(const size_t i_numBits, HeapManager* i_pAllocator, const bool i_startClear)
{
	assert(i_numBits > 0 && i_pAllocator);

	BitArray* bitArray = new ((BitArray*)i_pAllocator->_alloc(sizeof(BitArray))) BitArray(i_numBits, i_startClear);

	return bitArray;
}

inline BitArray::~BitArray()
{
	delete(m_pBits);
}

inline bool BitArray::IsBitSet(const size_t i_bitNumber) const
{
	assert(0 <= i_bitNumber && i_bitNumber < numBits);
	size_t elementIndex = i_bitNumber / bitsPerElement;
	size_t bitIndex = i_bitNumber % bitsPerElement;
	return m_pBits[elementIndex] & (static_cast<t_BitData>(1) << bitIndex);
}

inline bool BitArray::IsBitClear(const size_t i_bitNumber) const
{
	return !IsBitSet(i_bitNumber);
}

inline void BitArray::SetBit(const size_t i_bitNumber)
{
	assert(0 <= i_bitNumber && i_bitNumber < numBits);
	size_t elementIndex = i_bitNumber / bitsPerElement;
	size_t bitIndex = i_bitNumber % bitsPerElement;
	m_pBits[elementIndex] |= static_cast<t_BitData>(1) << static_cast<t_BitData>(bitIndex);
}

inline void BitArray::ClearBit(const size_t i_bitNumber)
{
	assert(0 <= i_bitNumber && i_bitNumber < numBits);
	size_t elementIndex = i_bitNumber / bitsPerElement;
	size_t bitIndex = i_bitNumber % bitsPerElement;
	m_pBits[elementIndex] &= ~(static_cast<t_BitData>(1) << static_cast<t_BitData>(bitIndex));
}

inline void BitArray::SetAll()
{
	memset(m_pBits, -1, (numBits % bitsPerElement == 0 && numBits / bitsPerElement != 0 ? numBits / bitsPerElement : (numBits / bitsPerElement) + 1) * sizeof(t_BitData));
}

inline void BitArray::ClearAll()
{
	memset(m_pBits, 0, (numBits % bitsPerElement == 0 && numBits / bitsPerElement != 0 ? numBits / bitsPerElement : (numBits / bitsPerElement) + 1) * sizeof(t_BitData));
}

inline bool BitArray::GetFirstClearBit(size_t& o_bitNumber) const
{
	size_t numElements = numBits % bitsPerElement == 0 && numBits / bitsPerElement != 0 ? numBits / bitsPerElement : (numBits / bitsPerElement) + 1;
	size_t elementIndex = 0;
	size_t bitIndex = 0;
	for (elementIndex = 0; elementIndex < numElements; elementIndex++)
	{
		if (BitScanForward(reinterpret_cast<unsigned long*>(&bitIndex), ~m_pBits[elementIndex]))
		{
			o_bitNumber = elementIndex * bitsPerElement + bitIndex;
			return true;
		}
	}

	return false;
}

inline bool BitArray::GetFirstSetBit(size_t& o_bitNumber) const
{
	size_t numElements = numBits % bitsPerElement == 0 && numBits / bitsPerElement != 0 ? numBits / bitsPerElement : (numBits / bitsPerElement) + 1;
	size_t elementIndex = 0;
	size_t bitIndex = 0;
	for (elementIndex = 0; elementIndex < numElements; elementIndex++)
	{
		if (BitScanForward(reinterpret_cast<unsigned long*>(&bitIndex), m_pBits[elementIndex]))
		{
			o_bitNumber = elementIndex * bitsPerElement + bitIndex;
			return true;
		}
	}

	return false;
}