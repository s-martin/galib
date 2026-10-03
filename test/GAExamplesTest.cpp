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
#include <garandom.h>


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

BOOST_AUTO_TEST_CASE(GAex2)
{
	auto ga = example2(102, true);

	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(0), 0.0980392173, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(1), 21.9607849, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(2), 3, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(3), -3.52941179, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(4), 34575.293, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(5), 0.00152941176, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(6), 6.56078434, 0.0000001);
}

BOOST_AUTO_TEST_CASE(GAex3)
{
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	BOOST_REQUIRE(params.set(gaNpCrossover, 0.8));
	BOOST_REQUIRE(params.set(gaNpMutation, 0.001));
	BOOST_REQUIRE(params.set(gaNflushFrequency, 50));
	BOOST_REQUIRE(params.set(gaNscoreFilename, "bog.dat"));

	GAResetRNG(103);
	auto ga = example3(params, "smiley.txt");

	BOOST_CHECK_EQUAL(ga.maxEver(), 192);
	BOOST_CHECK_EQUAL(ga.minEver(), 100);
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
	auto ga = example6(params6(), 106);

	BOOST_CHECK_EQUAL(ga.bestIndividual().size(), 10557);
	BOOST_CHECK_EQUAL(ga.bestIndividual().depth(), 310);
}

BOOST_AUTO_TEST_CASE(GAex7)
{
	GAResetRNG(107);
	auto ga = example7(params7(), "smiley.txt");

	BOOST_CHECK_EQUAL(ga.maxEver(), 217);
	BOOST_CHECK_EQUAL(ga.minEver(), 97);
	BOOST_CHECK_EQUAL(ga.generation(), 160);
}

BOOST_AUTO_TEST_CASE(GAex8)
{
	auto ga = example8(108);

	BOOST_CHECK_EQUAL(ga.bestIndividual().size(), 413);
}

BOOST_AUTO_TEST_CASE(GAex9)
{
	auto ga = example9(109);

	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(0), -7.62939453e-05, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(1), -7.62939453e-05, 0.0000001);
}

BOOST_AUTO_TEST_SUITE_END()
