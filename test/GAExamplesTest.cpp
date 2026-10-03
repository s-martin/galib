#include <boost/test/unit_test.hpp>

#include "ex1.hpp"
#include "ex2.hpp"
#include "ex3.hpp"
#include "ex4.hpp"
#include "ex6.hpp"
#include "ex7.hpp"
#include "ex8.hpp"
#include "ex9.hpp"

#include <fstream>
#include <iostream>


namespace
{
GAParameterList params6()
{
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNpopulationSize, 30);
	params.set(gaNpCrossover, 0.7);
	params.set(gaNpMutation, 0.01);
	params.set(gaNnGenerations, 100);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 10);
	return params;
}

GAParameterList params7()
{
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNpopulationSize, 30);
	params.set(gaNpCrossover, 0.9);
	params.set(gaNpMutation, 0.001);
	params.set(gaNnGenerations, 400);
	params.set(gaNpReplacement, 0.5);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	return params;
}
}

BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GAex1)
{
	auto ga = example1(0, true);

	std::stringstream str;
	str << ga.bestIndividual();
	BOOST_CHECK_EQUAL(str.str(), "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n");
}

// The seed is derived from the clock when 0 is passed, so the stochastic
// examples below are checked for plausible results rather than golden values.
BOOST_AUTO_TEST_CASE(GAex2)
{
	auto ga = example2(0, true);
	const auto& g = static_cast<const GABin2DecGenome&>(ga.bestIndividual());

	BOOST_CHECK_EQUAL(g.nPhenotypes(), 7u);
	BOOST_CHECK_EQUAL(g.phenotype(2), 3);
	for (unsigned int i = 0; i < g.nPhenotypes(); i++)
	{
		BOOST_CHECK_GE(g.phenotype(i), -10.0);
		BOOST_CHECK_LE(g.phenotype(i), 100000.0);
	}
	BOOST_CHECK_GE(ga.maxEver(), ga.minEver());
}

BOOST_AUTO_TEST_CASE(GAex3)
{
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	BOOST_REQUIRE(params.set(gaNpCrossover, 0.8));
	BOOST_REQUIRE(params.set(gaNpMutation, 0.001));
	BOOST_REQUIRE(params.set(gaNflushFrequency, 50));
	BOOST_REQUIRE(params.set(gaNscoreFilename, "bog.dat"));

	auto ga = example3(params, "smiley.txt");

	BOOST_CHECK_GT(ga.maxEver(), ga.minEver());
	BOOST_CHECK_GT(ga.maxEver(), 100);
	BOOST_CHECK_EQUAL(ga.generation(), 250);
}

BOOST_AUTO_TEST_CASE(GAex4)
{
	auto ga = example4(0);

	std::stringstream str;
	str << ga.bestIndividual();
	BOOST_CHECK_EQUAL(
		str.str(),
		"0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"
		"1010101010\n0101010101\n1010101010\n0101010101\n1010101010\n\n"
		"0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"
	);
}

BOOST_AUTO_TEST_CASE(GAex6)
{
	auto ga = example6(params6(), 0);
	const auto& g = static_cast<const GATreeGenome<int>&>(ga.bestIndividual());

	BOOST_CHECK_GT(g.size(), 0);
	BOOST_CHECK_GT(g.depth(), 0);
	BOOST_CHECK_LE(g.depth(), g.size());
}

BOOST_AUTO_TEST_CASE(GAex7)
{
	auto ga = example7(params7(), "smiley.txt");

	BOOST_CHECK_GT(ga.maxEver(), ga.minEver());
	BOOST_CHECK_GT(ga.generation(), 0);
	BOOST_CHECK_LE(ga.generation(), 400);
}

BOOST_AUTO_TEST_CASE(GAex8)
{
	auto ga = example8(0);

	BOOST_CHECK_GE(static_cast<const GAListGenome<int>&>(ga.bestIndividual()).size(), 0);
}

BOOST_AUTO_TEST_CASE(GAex9)
{
	auto ga = example9(0);
	const auto& g = static_cast<const GABin2DecGenome&>(ga.bestIndividual());

	// the optimum is (0, 0); the GA must at least end up close to it
	BOOST_CHECK_SMALL(g.phenotype(0), 1.0f);
	BOOST_CHECK_SMALL(g.phenotype(1), 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()
