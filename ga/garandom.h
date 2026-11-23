/* ----------------------------------------------------------------------------
  garandom.h
  mbwall 29jun95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  Random number generation for GAlib.  All functions use one global
std::mt19937 engine from the C++ standard library.  Seed it with GARandomSeed
(or GAResetRNG) to get reproducible results.

GARandomInt, GARandomFloat, GARandomDouble
  Return a number selected at random within the bounds low and high, inclusive
for integers.  Don't forget the 'inclusive' part!  If you're using this to get
the index of an element in an array, be sure to use the last element of the
array, not the number of elements in the array as the arguement for 'high'.
  The floating point versions without arguments return a number in [0, 1).
  If low is greater than high the behavior is undefined.

GARandomBit
  Returns 0 or 1 with equal probability.

GAFlipCoin
  Simulate a coin toss.  Use specified probability to bias toss.

GAUnitGaussian
  Returns a number from a Gaussian distribution with mean 0 and stddev of 1

GAGaussianFloat, GAGaussianDouble
  Scaled versions of the gaussian distribution.  You must specify a stddev,
then these functions scale the distribution to that deviation.  Mean is still 0
---------------------------------------------------------------------------- */
#pragma once

#include <gaconfig.h>
#include <gatypes.h>

/** Uniformly distributed double in [0, 1). */
double GARandomDouble();
/** Uniformly distributed float in [0, 1). */
float GARandomFloat();
/** Uniformly distributed integer in [low, high] (both inclusive). */
int GARandomInt(int low, int high);
/** Returns 0 or 1 with equal probability. */
int GARandomInt();
/** Uniformly distributed double in [low, high). */
double GARandomDouble(double low, double high);
/** Uniformly distributed float in [low, high). */
float GARandomFloat(float low, float high);

unsigned int GAGetRandomSeed();
void GARandomSeed(unsigned int seed = 0);
void GAResetRNG(unsigned int seed);
int GARandomBit();
double GAUnitGaussian();

inline bool GAFlipCoin(float p)
{
	return ((p == 1.0)
				? true
				: (p == 0.0) ? false : ((GARandomFloat() <= p) ? true : false));
}

inline float GAGaussianFloat(float dev)
{
	return static_cast<float>(GAUnitGaussian()) * dev;
}
inline double GAGaussianDouble(double dev) { return GAUnitGaussian() * dev; }

/** Name of the random number generator used by the library. */
const char *GAGetRNG();
