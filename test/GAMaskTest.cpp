#include <boost/test/unit_test.hpp>

#include <GAMask.h>

#include <exception>


BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GAMask_001)
{
	GAMask mask1;
	BOOST_CHECK_EQUAL(mask1.size(10), 10);
	BOOST_CHECK_EQUAL(mask1.size(), 10);

	mask1.mask(0) = 1;
	mask1.mask(1) = 0;
	mask1.mask(2) = 0;
	mask1.mask(3) = 0;
	mask1.mask(4) = 1;
	mask1.mask(5) = 0;
	mask1.mask(6) = 1;
	mask1.mask(7) = 1;
	mask1.mask(8) = 1;
	mask1.mask(9) = 1;

	BOOST_CHECK_EQUAL(mask1.mask(0), 1);
	BOOST_CHECK_EQUAL(mask1.mask(1), 0);
	BOOST_CHECK_EQUAL(mask1.mask(2), 0);
	BOOST_CHECK_EQUAL(mask1.mask(3), 0);
	BOOST_CHECK_EQUAL(mask1.mask(4), 1);
	BOOST_CHECK_EQUAL(mask1.mask(5), 0);
	BOOST_CHECK_EQUAL(mask1.mask(6), 1);
	BOOST_CHECK_EQUAL(mask1.mask(7), 1);
	BOOST_CHECK_EQUAL(mask1.mask(8), 1);
	BOOST_CHECK_EQUAL(mask1.mask(9), 1);

	// test copy
	GAMask mask3 = mask1;
	BOOST_CHECK_EQUAL(mask3.mask(0), 1);
	BOOST_CHECK_EQUAL(mask3.mask(1), 0);
	BOOST_CHECK_EQUAL(mask3.mask(2), 0);
	BOOST_CHECK_EQUAL(mask3.mask(3), 0);
	BOOST_CHECK_EQUAL(mask3.mask(4), 1);
	BOOST_CHECK_EQUAL(mask3.mask(5), 0);
	BOOST_CHECK_EQUAL(mask3.mask(6), 1);
	BOOST_CHECK_EQUAL(mask3.mask(7), 1);
	BOOST_CHECK_EQUAL(mask3.mask(8), 1);
	BOOST_CHECK_EQUAL(mask3.mask(9), 1);

	mask1.clear();
	BOOST_CHECK_EQUAL(mask1.mask(0), 0);
	BOOST_CHECK_EQUAL(mask1.mask(1), 0);
	BOOST_CHECK_EQUAL(mask1.mask(2), 0);
	BOOST_CHECK_EQUAL(mask1.mask(3), 0);
	BOOST_CHECK_EQUAL(mask1.mask(4), 0);
	BOOST_CHECK_EQUAL(mask1.mask(5), 0);
	BOOST_CHECK_EQUAL(mask1.mask(6), 0);
	BOOST_CHECK_EQUAL(mask1.mask(7), 0);
	BOOST_CHECK_EQUAL(mask1.mask(8), 0);
	BOOST_CHECK_EQUAL(mask1.mask(9), 0);

	GAMask mask2;
	BOOST_CHECK_EQUAL(mask2.size(2), 2);
	BOOST_CHECK_THROW(mask2.mask(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(GAMask_002)
{
	GAMask mask1;
	BOOST_CHECK_EQUAL(mask1.size(10), 10);
	BOOST_CHECK_EQUAL(mask1.size(), 10);

	mask1[0] = 1;
	mask1[1] = 0;
	mask1[2] = 0;
	mask1[3] = 0;
	mask1[4] = 1;
	mask1[5] = 0;
	mask1[6] = 1;
	mask1[7] = 1;
	mask1[8] = 1;
	mask1[9] = 1;

	BOOST_CHECK_EQUAL(mask1[0], 1);
	BOOST_CHECK_EQUAL(mask1[1], 0);
	BOOST_CHECK_EQUAL(mask1[2], 0);
	BOOST_CHECK_EQUAL(mask1[3], 0);
	BOOST_CHECK_EQUAL(mask1[4], 1);
	BOOST_CHECK_EQUAL(mask1[5], 0);
	BOOST_CHECK_EQUAL(mask1[6], 1);
	BOOST_CHECK_EQUAL(mask1[7], 1);
	BOOST_CHECK_EQUAL(mask1[8], 1);
	BOOST_CHECK_EQUAL(mask1[9], 1);

	// test copy
	GAMask mask3 = mask1;
	BOOST_CHECK_EQUAL(mask3[0], 1);
	BOOST_CHECK_EQUAL(mask3[1], 0);
	BOOST_CHECK_EQUAL(mask3[2], 0);
	BOOST_CHECK_EQUAL(mask3[3], 0);
	BOOST_CHECK_EQUAL(mask3[4], 1);
	BOOST_CHECK_EQUAL(mask3[5], 0);
	BOOST_CHECK_EQUAL(mask3[6], 1);
	BOOST_CHECK_EQUAL(mask3[7], 1);
	BOOST_CHECK_EQUAL(mask3[8], 1);
	BOOST_CHECK_EQUAL(mask3[9], 1);

	mask1.clear();
	BOOST_CHECK_EQUAL(mask1[0], 0);
	BOOST_CHECK_EQUAL(mask1[1], 0);
	BOOST_CHECK_EQUAL(mask1[2], 0);
	BOOST_CHECK_EQUAL(mask1[3], 0);
	BOOST_CHECK_EQUAL(mask1[4], 0);
	BOOST_CHECK_EQUAL(mask1[5], 0);
	BOOST_CHECK_EQUAL(mask1[6], 0);
	BOOST_CHECK_EQUAL(mask1[7], 0);
	BOOST_CHECK_EQUAL(mask1[8], 0);
	BOOST_CHECK_EQUAL(mask1[9], 0);

	GAMask mask2;
	BOOST_CHECK_EQUAL(mask2.size(2), 2);
	BOOST_CHECK_THROW(mask2.mask(2), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()