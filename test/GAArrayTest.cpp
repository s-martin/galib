#include <boost/test/unit_test.hpp>

#include <GAArray.h>
#include <iostream>


BOOST_AUTO_TEST_SUITE( UnitTest )

BOOST_AUTO_TEST_CASE(GAArray_001)
{
	GAArray<int> gaarray(2);
	BOOST_CHECK_EQUAL(gaarray.size(), 2);
	
	// TODO remove unsigned from operator
	BOOST_CHECK_EQUAL(gaarray[0u], 0);
	BOOST_CHECK_EQUAL(gaarray[1u], 0);

	int i1 = 1;
	int i2 = 2;
	gaarray[0u] = i1;
	gaarray[1u] = i2;
	
	BOOST_CHECK_EQUAL(gaarray[0u], 1);
	BOOST_CHECK_EQUAL(gaarray[1u], 2);

	auto gaarray2 = gaarray;
	BOOST_CHECK_EQUAL(gaarray2.size(), 2);
	BOOST_CHECK_EQUAL(gaarray2[0u], 1);
	BOOST_CHECK_EQUAL(gaarray2[1u], 2);

	//BOOST_CHECK(gaarray == gaarray2);

	GAArray gaarray3(gaarray);
	BOOST_CHECK_EQUAL(gaarray3.size(), 2);
	BOOST_CHECK_EQUAL(gaarray3[0u], 1);
	BOOST_CHECK_EQUAL(gaarray3[1u], 2);

	int simplearray[2];
	simplearray[0] = 3;
	simplearray[1] = 4;
	BOOST_CHECK_EQUAL(simplearray[0u], 3);
	BOOST_CHECK_EQUAL(simplearray[1u], 4);

	GAArray<int> gaarray4(2);
	gaarray4 = simplearray;
	BOOST_CHECK_EQUAL(gaarray4.size(), 2);
	BOOST_CHECK_EQUAL(gaarray4[0u], 3);
	BOOST_CHECK_EQUAL(gaarray4[1u], 4);

	//BOOST_CHECK(gaarray != gaarray4);

	GAArray<int> gaarray5(0);
	gaarray5 = gaarray;
	gaarray5.swap(0, 1);
	BOOST_CHECK_EQUAL(gaarray5[0u], 2);
	BOOST_CHECK_EQUAL(gaarray5[1u], 1);

	// TODO add failures to improve implementation
}

BOOST_AUTO_TEST_SUITE_END()