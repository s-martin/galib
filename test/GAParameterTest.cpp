#include <boost/test/unit_test.hpp>

#include <GABaseGA.h>
#include <GAParameter.h>

BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GAParameterList_001)
{
	GAParameterList params;
	BOOST_CHECK_EQUAL(50, params.populationSize);
	BOOST_CHECK_CLOSE_FRACTION(0.9, params.probMutation, 0.001);
	BOOST_CHECK_CLOSE_FRACTION(0.01, params.probCrossover, 0.01);
	BOOST_CHECK_EQUAL("generations.dat", params.scoreFilename);
	BOOST_CHECK_EQUAL(1, params.scoreFrequency);
	BOOST_CHECK_EQUAL(0, params.flushFrequency);
	BOOST_CHECK_EQUAL(2, params.selectScores);
	BOOST_CHECK_EQUAL(250, params.numGenerations);
}

BOOST_AUTO_TEST_CASE(read_001)
{
	GAParameterList params;
	BOOST_REQUIRE(params.read("settings.txt", false));
	
	BOOST_CHECK_EQUAL(100, params.populationSize);
	BOOST_CHECK_CLOSE_FRACTION(0.01, params.probMutation, 0.001);
	BOOST_CHECK_CLOSE_FRACTION(0.9, params.probCrossover, 0.01);
	BOOST_CHECK_EQUAL("bog.dat", params.scoreFilename);
	BOOST_CHECK_EQUAL(1, params.scoreFrequency);
	BOOST_CHECK_EQUAL(25, params.flushFrequency);
	BOOST_CHECK_EQUAL(7, params.selectScores);
	BOOST_CHECK_EQUAL(10, params.numGenerations);
}

BOOST_AUTO_TEST_SUITE_END()