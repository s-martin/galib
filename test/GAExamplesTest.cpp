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
	auto ga = example2(0, true);

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

	auto ga = example3(params, "smiley.txt");

	// TODO older MSVC versions somehow differed; maybe seed is different
//#ifdef _WIN32
//	BOOST_CHECK_EQUAL(ga.statistics().maxEver(), 198);
//	BOOST_CHECK_EQUAL(ga.statistics().minEver(), 98);
//#else
	BOOST_CHECK_EQUAL(ga.statistics().maxEver(), 192);
	BOOST_CHECK_EQUAL(ga.statistics().minEver(), 100);
//#endif
	BOOST_CHECK_EQUAL(ga.statistics().generation(), 250);
}

BOOST_AUTO_TEST_CASE(GAex4)
{
	auto ga = example4(0, true);

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
	auto ga = example6(0, true);

	BOOST_CHECK_EQUAL(ga.bestIndividual().size(), 10557);
	BOOST_CHECK_EQUAL(ga.bestIndividual().depth(), 310);
}

BOOST_AUTO_TEST_CASE(GAex7)
{
	auto ga = example7(0, true);

	BOOST_CHECK_EQUAL(ga.statistics().maxEver(), 217);
	BOOST_CHECK_EQUAL(ga.statistics().minEver(), 97);
	BOOST_CHECK_EQUAL(ga.statistics().generation(), 160);
}

BOOST_AUTO_TEST_CASE(GAex8)
{
	auto ga = example8(0, true);

	BOOST_CHECK_EQUAL(ga.bestIndividual().size(), 413);
}

BOOST_AUTO_TEST_CASE(GAex9)
{
	auto ga = example9(0, true);

	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(0), -7.62939453e-05, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(ga.bestIndividual().phenotype(1), -7.62939453e-05, 0.0000001);
}

BOOST_AUTO_TEST_SUITE_END()
