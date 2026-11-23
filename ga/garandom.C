/* ----------------------------------------------------------------------------
  garandom.C
  mbwall 5sep95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  Random number stuff for use in GAlib, based on the C++ standard library.
---------------------------------------------------------------------------- */
#include <garandom.h>

#include <cmath>
#include <random>

namespace
{
std::mt19937 &engine()
{
	static std::mt19937 e{std::random_device{}()};
	return e;
}

std::normal_distribution<double> &gaussian()
{
	static std::normal_distribution<double> d{0.0, 1.0};
	return d;
}

unsigned int seed = 0;

void applySeed(unsigned int s)
{
	seed = s;
	engine().seed(s);
	gaussian().reset(); // forget the cached twin of the previous seed
}
} // namespace

const char *GAGetRNG() { return "std::mt19937"; }

// Seed the random number generator.  If a seed is specified, then set the
// generator to that seed.  We remember the seed so that multiple calls with the
// same seed do not reset the generator.  Subsequent calls with a different
// seed will initialize the generator to the new seed.  Multiple calls with a
// value of 0 do nothing (we do *not* re-seed the generator because 0 is the
// default value and we don't want people to re-seed the generator
// inadvertantly).  The first call with 0 picks a seed from the system.
unsigned int GAGetRandomSeed() { return seed; }

void GARandomSeed(unsigned int s)
{
	if (s == 0 && seed == 0)
	{
		std::random_device rd;
		unsigned int picked = 0;
		while (picked == 0)
			picked = rd();
		applySeed(picked);
	}
	else if (s != 0 && seed != s)
	{
		applySeed(s);
	}
}

// Similar to setting the random seed, but this one sets it as long as the
// specified seed is non-zero.
void GAResetRNG(unsigned int s)
{
	if (s != 0)
		applySeed(s);
}

double GARandomDouble()
{
	return std::uniform_real_distribution<double>(0.0, 1.0)(engine());
}

float GARandomFloat()
{
	float val = std::uniform_real_distribution<float>(0.0F, 1.0F)(engine());
	// generate_canonical may round up to exactly 1.0
	return val < 1.0F ? val : std::nextafter(1.0F, 0.0F);
}

int GARandomInt() { return GARandomBit(); }

int GARandomInt(int low, int high)
{
	return std::uniform_int_distribution<int>(low, high)(engine());
}

double GARandomDouble(double low, double high)
{
	return low + (high - low) * GARandomDouble();
}

float GARandomFloat(float low, float high)
{
	return low + (high - low) * GARandomFloat();
}

int GARandomBit() { return static_cast<int>(engine()() & 1U); }

// Return a number from a unit Gaussian distribution.  The mean is 0 and the
// standard deviation is 1.0.
double GAUnitGaussian() { return gaussian()(engine()); }
