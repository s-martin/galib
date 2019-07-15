// $Header$
/* ----------------------------------------------------------------------------
  gatypes.h
  mbwall 29apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_types_h_
#define _ga_types_h_

using GAProbability = float;
using GAProb = float;

// This defines what type to use for the bitstream data storage.  Use the
// smallest type available on your platform.
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

#endif
