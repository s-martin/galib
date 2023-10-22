/* ----------------------------------------------------------------------------
  mbwall 29apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

/// Type for the bitstream data storage.
using GABit = unsigned char;

#ifdef _WIN32 // for GCC already defined
const auto M_PI = 3.14159265358979323846;
#endif

template <typename T1, typename T2> constexpr auto GAMax(T1 a, T2 b)
{
	return (((a) > (b)) ? (a) : (b));
}

template <typename T1, typename T2> constexpr auto GAMin(T1 a, T2 b)
{
	return (((a) < (b)) ? (a) : (b));
}
