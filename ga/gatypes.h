// $Header$
/* ----------------------------------------------------------------------------
  gatypes.h
  mbwall 29apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_types_h_
#define _ga_types_h_

typedef float GAProbability;
typedef float GAProb;


// This defines what type to use for the bitstream data storage.  Use the
// smallest type available on your platform.
typedef unsigned char GABit;

#define GAMax(a, b) ((a) > (b) ? (a) : (b))
#define GAMin(a, b) ((a) < (b) ? (a) : (b))

#endif
