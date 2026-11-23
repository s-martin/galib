#include <boost/test/unit_test.hpp>

#include <cmath>
#include <cstring>
#include <garandom.h>

#include <fstream>
#include <iostream>

static const int HIST_SIZE = 501;
static const int NFLIPS = 10000;
static const long int NUM_CHECKS = 1000000L;
static const int RCHI = 100;

BOOST_AUTO_TEST_SUITE(UnitTest)

// test the random number generator in GAlib
// do the chi square test
BOOST_AUTO_TEST_CASE(Randtest_001)
{
	std::cerr << "This program checks the random number generators in GAlib.\n";
	std::cerr << "These are not rigorous statistical tests, but they should\n";
	std::cerr
		<< "give you a quick indication of whether or not the library is\n";
	std::cerr << "working properly.  Command-line options include:\n\n";
	std::cerr << "  [+-]chi    - do the chi square test (default)\n";
	std::cerr
		<< "  [+-]hist   - do the histogram tests.  Histograms should be\n";
	std::cerr
		<< "               nearlyflat, i.e. each number has an equal chance\n";
	std::cerr << "               of beingchosen.\n";
	std::cerr << "  [+-]bnds   - do the bounds tests.  All numbers generated\n";
	std::cerr
		<< "               should be within the limits specified in the\n";
	std::cerr << "               test.\n";
	std::cerr
		<< "  [+-]means  - do the means tests.  A few thousand invocations\n";
	std::cerr
		<< "               of each random number function are made, and the\n";
	std::cerr
		<< "               averages of these calls are displayed with the\n";
	std::cerr << "               number that should have been found.\n";
	std::cerr
		<< "      seed n - specify the seed number to use for the RNG.  You\n";
	std::cerr
		<< "               should get the same results every time if you\n";
	std::cerr
		<< "               specify the same seed every time.  A seed of 0\n";
	std::cerr
		<< "               tells the lib to pick its own seed based upon the\n";
	std::cerr << "               current time.\n";
	std::cerr << "\n";
	// cerr.flush();

	int seed = 0;

	// Tell us which RNG we're using...
	std::cout << "Random Number Test\n";
	std::cout << "Using the " << GAGetRNG()
			  << " random number generator (RNG).\n";

	// initialize the RNG by calling the seed routine with our seed
	if (seed)
	{
		std::cout << "Using specified random seed " << seed << "\n";
	}
	else
	{
		std::cout << "No random seed specified\n";
	}

	GARandomSeed(seed);

	unsigned int libseed = GAGetRandomSeed();
	std::cout << "Library thinks the random seed is " << libseed << "\n";

	// the basic chi square test for randomness of a RNG
	// must do it more than once since it might be wrong about one in ten
	// times.  The error is r*t/N - N and should be within 2*sqrt(r) of r.

	std::cout << "\n";
	std::cerr << "chi-square test...\n";
	std::cout << "running the chi-square test for randomness of the RNG...\n";
	std::cout << " (there will be some failures of the chi-square test)\n";

	int ii;
	double elimit = 2 * sqrt((double)RCHI);
	double chisq = 0.0;
	long int NCHI = 1000; // NCHI should be >= 10 * RCHI
	long int f[RCHI];

	std::cerr << "  integer test (" << NCHI << ")...\n";
	std::cout << "  integer test (" << NCHI
			  << "):  chi-squared should be within ";
	std::cout << elimit << " of " << RCHI << "\n";
	std::cout.flush();
	for (ii = 0; ii < 10; ii++)
	{
		memset(f, 0, RCHI * sizeof(long int));
		for (int i = 0; i < NCHI; i++)
			f[GARandomInt(0, RCHI - 1)]++;
		for (int i = 0; i < RCHI; i++)
			chisq += ((double)f[i] - (double)NCHI / (double)RCHI) *
					 ((double)f[i] - (double)NCHI / (double)RCHI);
		chisq *= (double)RCHI / (double)NCHI;
		std::cout << "    run #" << ii << "\t" << chisq << "\t";
		BOOST_CHECK(!(fabs(chisq - RCHI) > elimit));
	}
	std::cout.flush();

	NCHI = 10000;
	std::cerr << "  integer test (" << NCHI << ")...\n";
	std::cout << "  integer test (" << NCHI
			  << "):  chi-squared should be within ";
	std::cout << elimit << " of " << RCHI << "\n";
	std::cout.flush();
	for (ii = 0; ii < 10; ii++)
	{
		memset(f, 0, RCHI * sizeof(long int));
		long int i;
		long int t;
		for (i = 0; i < NCHI; i++)
			f[GARandomInt(0, RCHI - 1)]++;
		for (i = 0, t = 0; i < RCHI; i++)
			t += f[i] * f[i];
		chisq = (double)RCHI * (double)t / (double)NCHI - (double)NCHI;
		std::cout << "    run #" << ii << "\t" << chisq << "\t";
		BOOST_CHECK(!(fabs(chisq - RCHI) > elimit));
	}
	std::cout.flush();

	NCHI = 10000;
	std::cerr << "  integer test (" << NCHI << ")...\n";
	std::cout << "  integer test (" << NCHI
			  << "):  chi-squared should be within ";
	std::cout << elimit << " of " << RCHI << "\n";
	std::cout.flush();
	for (ii = 0; ii < 10; ii++)
	{
		memset(f, 0, RCHI * sizeof(long int));
		long int i;
		long int t;
		for (i = 0; i < NCHI; i++)
			f[GARandomInt(0, RCHI - 1)]++;
		for (i = 0, t = 0; i < RCHI; i++)
			t += f[i] * f[i];
		chisq = (double)RCHI * (double)t / (double)NCHI - (double)NCHI;
		std::cout << "    run #" << ii << "\t" << chisq << "\t";
		BOOST_CHECK(!(fabs(chisq - RCHI) > elimit));
	}
	std::cout.flush();

	NCHI = 100000;
	std::cerr << "  integer test (" << NCHI << ")...\n";
	std::cout << "  integer test (" << NCHI
			  << "):  chi-squared should be within ";
	std::cout << elimit << " of " << RCHI << "\n";
	std::cout.flush();
	for (ii = 0; ii < 10; ii++)
	{
		memset(f, 0, RCHI * sizeof(long int));
		long int i;
		long int t;
		for (i = 0; i < NCHI; i++)
			f[GARandomInt(0, RCHI - 1)]++;
		for (i = 0, t = 0; i < RCHI; i++)
			t += f[i] * f[i];
		chisq = (double)RCHI * (double)t / (double)NCHI - (double)NCHI;
		std::cout << "    run #" << ii << "\t" << chisq << "\t";
		BOOST_CHECK(!(fabs(chisq - RCHI) > elimit));
	}
	std::cout.flush();
}

// histograms of the tests
BOOST_AUTO_TEST_CASE(Randtest_002)
{
	std::cout << "\n";
	std::cerr << "histograms...\n";
	std::cout << "plotting histograms of calls to random number functions...\n";

	int i, j, histogram[HIST_SIZE];

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout << "\n100 random integers in [0, 1] with GARandomInt():\n";
	for (i = 0; i < 100; i++)
		histogram[GARandomInt()]++;
	for (i = 0; i <= 1; i++)
	{
		std::cout << i << "\t";
		for (j = 0; j < histogram[i]; j++)
			std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout << "\n100 random integers in [0, 1] with GARandomInt(0,1):\n";
	for (i = 0; i < 100; i++)
		histogram[GARandomInt(0, 1)]++;
	for (i = 0; i <= 1; i++)
	{
		std::cout << i << "\t";
		for (j = 0; j < histogram[i]; j++)
			std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout << "\n10000 random integers in [-20, -10] with "
				 "GARandomInt(-20,-10):\n";
	for (i = 0; i < 10000; i++)
		histogram[GARandomInt(-20, -10) + 20]++;
	for (i = 0; i <= 10; i++)
	{
		std::cout << (i - 20) << "\t";
		for (j = 0; j < histogram[i]; j++)
			if (j % 20 == 0)
				std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout << "\n300 random integers in [0, 5] with GARandomInt(0,5):\n";
	for (i = 0; i < 300; i++)
		histogram[GARandomInt(0, 5)]++;
	for (i = 0; i <= 5; i++)
	{
		std::cout << i << "\t";
		for (j = 0; j < histogram[i]; j++)
			std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout
		<< "\n10000 random integers in [0, 100] with GARandomInt(0,100):\n";
	for (i = 0; i < 10000; i++)
		histogram[GARandomInt(0, 100)]++;
	for (i = 0; i <= 100; i++)
	{
		std::cout << i << "\t";
		for (j = 0; j < histogram[i]; j++)
			if (j % 2 == 0)
				std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();

	memset(histogram, 0, HIST_SIZE * sizeof(int));
	std::cout << "\n10000 random integers in [-10, 100] with "
				 "GARandomInt(-10,100):\n";
	for (i = 0; i < 10000; i++)
		histogram[GARandomInt(-10, 100) + 10]++;
	for (i = 0; i <= 110; i++)
	{
		std::cout << (i - 10) << "\t";
		for (j = 0; j < histogram[i]; j++)
			if (j % 2 == 0)
				std::cout << ".";
		std::cout << "\n";
	}
	std::cout.flush();
}

// calculate means on various types

BOOST_AUTO_TEST_CASE(Randtest_003)
{

	std::cout << "\n";
	std::cerr << "means test...\n";
	std::cout << "check for averages of repeated biased coin tosses...\n";

	int i;
	double counter, mean;

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomBit()\t\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomBit();
	std::cout << counter << " hits (should be about " << 0.5 * NFLIPS << ")\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomInt()\t\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomInt();
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 0.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomInt(0,5)\t\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomInt(0, 5);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 2.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomFloat()\t\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomFloat();
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 0.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomFloat(0,5)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomFloat(0, 5);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 2.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomFloat(-10,-5)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomFloat(-10, -5);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about -7.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomFloat(-10,10)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomFloat(-10, 10);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 0.0)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomDouble()\t\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomDouble();
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 0.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomDouble(0,2)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomDouble(0, 2);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 1.0)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomDouble(0,5)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomDouble(0, 5);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 2.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomDouble(-10,-5)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomDouble(-10, -5);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about -7.5)\n";
	std::cout.flush();

	counter = 0;
	std::cout << NFLIPS << " calls to GARandomDouble(-10,10)\t";
	for (i = 0; i < NFLIPS; i++)
		counter += GARandomDouble(-10, 10);
	mean = counter / (double)NFLIPS;
	std::cout << mean << " (should be about 0.0)\n";
	std::cout.flush();

	// check means of repeated coin tosses

	std::cout << "\n";
	float ave;
	std::cout << "[0, 1] with GARandomFloat()...\t\t";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat();
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat();
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat();
	std::cout << ave / 50 << "\n";
	std::cout.flush();

	std::cout << "[0, 1] with GARandomFloat(0,1)...\t";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 1);
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 1);
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 1);
	std::cout << ave / 50 << "\n";
	std::cout.flush();

	std::cout << "[0, 100] with GARandomFloat(0,100)...\t";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 100);
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 100);
	std::cout << ave / 50 << " ";
	ave = 0;
	for (i = 0; i < 50; i++)
		ave += GARandomFloat(0, 100);
	std::cout << ave / 50 << "\n";
	std::cout.flush();

	double aved;
	std::cout << "[0, 1] with GARandomDouble()...\t\t";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble();
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble();
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble();
	std::cout << aved / 50 << "\n";
	std::cout.flush();

	std::cout << "[0, 1] with GARandomDouble(0,1)...\t";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 1);
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 1);
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 1);
	std::cout << aved / 50 << "\n";
	std::cout.flush();

	std::cout << "[0, 100] with GARandomDouble(0,100)...\t";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 100);
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 100);
	std::cout << aved / 50 << " ";
	aved = 0;
	for (i = 0; i < 50; i++)
		aved += GARandomDouble(0, 100);
	std::cout << aved / 50 << "\n";
	std::cout.flush();

	std::cout << "\n";

	int count;
	std::cout << "1% coin flip...\t\t";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.01);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.01);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.01);
	counter = (double)count / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();

	std::cout << "25% coin flip...\t";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.25);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.25);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.25);
	counter = (double)count / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();

	std::cout << "50% coin flip...\t";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.5);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.5);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.5);
	counter = (double)count / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();

	std::cout << "75% coin flip...\t";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.75);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.75);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.75);
	counter = (double)count / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();

	std::cout << "99% coin flip...\t";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.99);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.99);
	counter = (double)count / (double)i;
	std::cout << counter << " ";
	count = 0;
	for (i = 0; i < NFLIPS; i++)
		count += GAFlipCoin(0.99);
	counter = (double)count / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();

	long lcount;
	std::cout << "\ncalls to GARandomBit (these should yield 50%)...\n";
	lcount = 0;
	for (i = 0; i < NFLIPS; i++)
		lcount += GARandomBit();
	counter = (double)lcount / (double)i;
	std::cout << counter << " ";
	lcount = 0;
	for (i = 0; i < NFLIPS; i++)
		lcount += GARandomBit();
	counter = (double)lcount / (double)i;
	std::cout << counter << " ";
	lcount = 0;
	for (i = 0; i < NFLIPS; i++)
		lcount += GARandomBit();
	counter = (double)lcount / (double)i;
	std::cout << counter << "\n";
	std::cout.flush();
}

// check for values outside of bounds
BOOST_AUTO_TEST_CASE(Randtest_004)
{
	std::cout << "\n";
	std::cerr << "bounds test...\n";
	std::cout << "check for out of bounds...\n";

	long int ii, err;

	int vali;

	std::cerr << "  " << NUM_CHECKS << "  GARandomInt(-10,5)...\n";
	std::cout << "GARandomInt(-10,5) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vali = GARandomInt(-10, 5);
		if (vali < -10 || vali > 5)
		{
			err += 1;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomInt(0,3)...\n";
	std::cout << "GARandomInt(0,3) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vali = GARandomInt(0, 3);
		if (vali < 0 || vali > 3)
		{
			err += 1;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomInt(200,255)...\n";
	std::cout << "GARandomInt(200,255) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vali = GARandomInt(200, 255);
		if (vali < 200 || vali > 255)
		{
			err += 1;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	float valf, lastf = 0.0;

	std::cerr << "  " << NUM_CHECKS << "  GARandomFloat(-10,5)...\n";
	std::cout << "GARandomFloat(-10,5) ...";
	std::cout.flush();
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		valf = GARandomFloat(-10, 5);
		if (valf < -10 || valf > 5)
		{
			err += 1;
			lastf = valf;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomFloat(0,3)...\n";
	std::cout << "GARandomFloat(0,3) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		valf = GARandomFloat(0, 3);
		if (valf < 0 || valf > 3)
		{
			err += 1;
			lastf = valf;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomFloat(200,255)...\n";
	std::cout << "GARandomFloat(200,255) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		valf = GARandomFloat(200, 255);
		if (valf < 200 || valf > 255)
		{
			err += 1;
			lastf = valf;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	double vald, lastd = 0.0;

	std::cerr << "  " << NUM_CHECKS << "  GARandomDouble(-10,5)...\n";
	std::cout << "GARandomDouble(-10,5) ...";
	std::cout.flush();
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vald = GARandomDouble(-10, 5);
		if (vald < -10 || vald > 5)
		{
			err += 1;
			lastd = vald;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomDouble(0,3)...\n";
	std::cout << "GARandomDouble(0,3) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vald = GARandomDouble(0, 3);
		if (vald < 0 || vald > 3)
		{
			err += 1;
			lastd = vald;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();

	std::cerr << "  " << NUM_CHECKS << "  GARandomDouble(200,255)...\n";
	std::cout << "GARandomDouble(200,255) ...";
	std::cout.flush();
	err = 0;
	for (ii = 0; ii < NUM_CHECKS; ii++)
	{
		vald = GARandomDouble(200, 255);
		if (vald < 200 || vald > 255)
		{
			err += 1;
			lastd = vald;
		}
	}

	BOOST_CHECK_EQUAL(err, 0);
	std::cout.flush();
}

BOOST_AUTO_TEST_SUITE_END()