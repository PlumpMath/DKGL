//
//  File: DKEndianness.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2015 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypes.h"


#if !defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)
#error System endianness not defined.
#endif

namespace DKFoundation
{
	////////////////////////////////////////////////////////////////////////////////
	// byteorder swap template functions.

	FORCEINLINE uint8_t DKSwitchIntegralByteOrder(uint8_t n)
	{
		static_assert(sizeof(uint8_t) == 1, "Invalid type size");
		return n;
	}
	FORCEINLINE uint16_t DKSwitchIntegralByteOrder(uint16_t n)
	{
		static_assert(sizeof(uint16_t) == 2, "Invalid type size");
		return
			((n & 0xff00) >> 8) |
			((n & 0x00ff) << 8);
	}
	FORCEINLINE uint32_t DKSwitchIntegralByteOrder(uint32_t n)
	{
		static_assert(sizeof(uint32_t) == 4, "Invalid type size");
		return
			((n & 0xff000000) >> 24) |
			((n & 0x00ff0000) >> 8) |
			((n & 0x0000ff00) << 8) |
			((n & 0x000000ff) << 24);
	}
	FORCEINLINE uint64_t DKSwitchIntegralByteOrder(uint64_t n)
	{
		static_assert(sizeof(uint64_t) == 8, "Invalid type size");
		return
			((n & 0xff00000000000000ULL) >> 56) |
			((n & 0x00ff000000000000ULL) >> 40) |
			((n & 0x0000ff0000000000ULL) >> 24) |
			((n & 0x000000ff00000000ULL) >> 8) |
			((n & 0x00000000ff000000ULL) << 8) |
			((n & 0x0000000000ff0000ULL) << 24) |
			((n & 0x000000000000ff00ULL) << 40) |
			((n & 0x00000000000000ffULL) << 56);
	}
	template <typename T> FORCEINLINE T DKSwitchIntegralByteOrder(T n, DKNumber<1>)
	{
		static_assert(sizeof(T) == 1, "Invalid type size");
		auto r = DKSwitchIntegralByteOrder(reinterpret_cast<uint8_t&>(n));
		return reinterpret_cast<T&>(r);
	}
	template <typename T> FORCEINLINE T DKSwitchIntegralByteOrder(T n, DKNumber<2>)
	{
		static_assert(sizeof(T) == 2, "Invalid type size");
		auto r = DKSwitchIntegralByteOrder(reinterpret_cast<uint16_t&>(n));
		return reinterpret_cast<T&>(r);
	}
	template <typename T> FORCEINLINE T DKSwitchIntegralByteOrder(T n, DKNumber<4>)
	{
		static_assert(sizeof(T) == 4, "Invalid type size");
		auto r = DKSwitchIntegralByteOrder(reinterpret_cast<uint32_t&>(n));
		return reinterpret_cast<T&>(r);
	}
	template <typename T> FORCEINLINE T DKSwitchIntegralByteOrder(T n, DKNumber<8>)
	{
		static_assert(sizeof(T) == 8, "Invalid type size");
		auto r = DKSwitchIntegralByteOrder(reinterpret_cast<uint64_t&>(n));
		return reinterpret_cast<T&>(r);
	}

	// System -> Big-Endian
	template <typename T> FORCEINLINE T DKSystemToBigEndian(T n)
	{
		static_assert(std::is_integral<T>::value, "Argument must be integer.");
#ifdef __LITTLE_ENDIAN__
		return DKSwitchIntegralByteOrder(n, DKNumber<sizeof(T)>());
#endif
		return n;
	}
	// Big-Endian to System
	template <typename T> FORCEINLINE T DKBigEndianToSystem(T n)
	{
		static_assert(std::is_integral<T>::value, "Argument must be integer.");
#ifdef __LITTLE_ENDIAN__
		return DKSwitchIntegralByteOrder(n, DKNumber<sizeof(T)>());
#endif
		return n;
	}

	// System -> Little-Endian
	template <typename T> FORCEINLINE T DKSystemToLittleEndian(T n)
	{
		static_assert(std::is_integral<T>::value, "Argument must be integer.");
#ifdef __BIG_ENDIAN__
		return DKSwitchIntegralByteOrder(n, DKNumber<sizeof(T)>());
#endif
		return n;
	}
	// Little-Endian to System
	template <typename T> FORCEINLINE T DKLittleEndianToSystem(T n)
	{
		static_assert(std::is_integral<T>::value, "Argument must be integer.");
#ifdef __BIG_ENDIAN__
		return DKSwitchIntegralByteOrder(n, DKNumber<sizeof(T)>());
#endif
		return n;
	}


	////////////////////////////////////////////////////////////////////////////////
	// runtime byte order test.
	// using preprocessor macros at compile-time and validate in run-time.
	enum class DKByteOrder  // middle-endian is not supported.
	{
		Unknown,
		BigEndian,
		LittleEndian,
	};
	inline DKByteOrder DKRuntimeByteOrder(void)
	{
		union
		{
			uint8_t c[4];
			uint32_t i;
		} val = { 0x01, 0x02, 0x03, 0x04 };

		switch (val.i)
		{
			case 0x01020304U:	return DKByteOrder::BigEndian;		break;
			case 0x04030201U:	return DKByteOrder::LittleEndian;	break;
		}
		return DKByteOrder::Unknown;
	}

	inline bool DKVerifyByteOrder(void)
	{
#if     defined(__BIG_ENDIAN__)
		return DKRuntimeByteOrder() == DKByteOrder::BigEndian;
#elif  defined(__LITTLE_ENDIAN__)
		return DKRuntimeByteOrder() == DKByteOrder::LittleEndian;
#endif
	}
}
