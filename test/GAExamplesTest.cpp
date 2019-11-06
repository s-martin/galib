#include <boost/test/unit_test.hpp>

#include "ex1.hpp"
#include "ex2.hpp"
#include "ex4.hpp"
#include "ex7.hpp"
#include "ex9.hpp"

#include <fstream>
#include <iostream>


BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GAex1)
{
	auto ga = ex1();

	std::stringstream str;
	str << ga.statistics().bestIndividual();
	BOOST_CHECK_EQUAL(str.str(), "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n");
}

BOOST_AUTO_TEST_CASE(GAex2)
{
	auto genome = ex2(101, true); // use static seed

	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(0), 0.0980392173, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(1), 21.9607849, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(2), 3, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(3), -3.52941179, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(4), 34575.293, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(5), 0.00152941176, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(6), 6.56078434, 0.0000001);
}

BOOST_AUTO_TEST_CASE(GAex4)
{
	auto ga = ex4();

	std::stringstream str;
	str << ga.statistics().bestIndividual();
	BOOST_CHECK_EQUAL(
		str.str(),
		"0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"
		"1010101010\n0101010101\n1010101010\n0101010101\n1010101010\n\n"
		"0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"
	);
}


BOOST_AUTO_TEST_CASE(GAex7)
{
	GARandomSeed(100); // so test is always the same

	// Set the default values of the parameters.
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	BOOST_REQUIRE(params.set(gaNpopulationSize, 50)); // number of individuals in population
	BOOST_REQUIRE(params.set(gaNpCrossover, 0.8)); // likelihood of doing crossover
	BOOST_REQUIRE(params.set(gaNpMutation, 0.001)); // probability of mutation
	BOOST_REQUIRE(params.set(gaNnGenerations, 200)); // number of generations
	BOOST_REQUIRE(params.set(gaNscoreFrequency, 20)); // how often to record scores
	BOOST_REQUIRE(params.set(gaNflushFrequency, 50)); // how often to flush scores to file
	BOOST_REQUIRE(params.set(gaNscoreFilename, "bog.dat"));
	
	auto ga = ex7(params, "smiley.txt");

	BOOST_CHECK_EQUAL(ga.statistics().maxEver(), 217);
	BOOST_CHECK_EQUAL(ga.statistics().minEver(), 97);
	BOOST_CHECK_EQUAL(ga.statistics().generation(), 160);
}

BOOST_AUTO_TEST_CASE(GAex9)
{
	auto genome = ex9(100); // use static seed

	// TODO check, why msvc and gcc differ; maybe seed is different
#ifdef _WIN32
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(0), -0.000381469727, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(1), -0.00221252441, 0.0000001);
#else
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(0), -0.0025177002, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(1), -0.000839233398, 0.0000001);
#endif
}

BOOST_AUTO_TEST_SUITE_END()