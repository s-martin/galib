#include <boost/test/unit_test.hpp>

#include <GABinStr.hpp>


BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GABinaryString_001)
{
	GABinaryString binstr(3);
	BOOST_CHECK_EQUAL(binstr.size(), 3);

	BOOST_CHECK_EQUAL(binstr.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr.bit(1u), 0);
	BOOST_CHECK_EQUAL(binstr.bit(2u), 0);

	BOOST_CHECK_EQUAL(binstr.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr.bit(2u, 2), 1); // higher than 1 is converted to one

	BOOST_CHECK_EQUAL(binstr.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr.bit(2u), 1); // higher than 1 is converted to one
}

BOOST_AUTO_TEST_CASE(resize_001)
{
	GABinaryString binstr1(3);
	BOOST_CHECK_EQUAL(binstr1.size(), 3);

	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1);

	GABinaryString binstr2 = binstr1;
	BOOST_CHECK(binstr1.equal(binstr2, 0, 0, 3));

	binstr1.resize(2);
	BOOST_CHECK_EQUAL(binstr1.size(), 2);
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 1);

	binstr2.resize(4);
	BOOST_CHECK_EQUAL(binstr2.size(), 4);
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(3u), 0);
}

BOOST_AUTO_TEST_CASE(equal_001)
{
	GABinaryString binstr1(3);
	BOOST_CHECK_EQUAL(binstr1.size(), 3);

	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1); // higher than 1 is converted to one

	GABinaryString binstr2(3);
	BOOST_CHECK_EQUAL(binstr2.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(2u, 1), 1);
	BOOST_CHECK(binstr1.equal(binstr2, 0, 0, 3));

	GABinaryString binstr3(3);
	BOOST_CHECK_EQUAL(binstr3.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr3.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr3.bit(2u, 0), 0);
	BOOST_CHECK(!binstr1.equal(binstr3, 0, 0, 3));

	GABinaryString binstr4(2);
	BOOST_CHECK_EQUAL(binstr4.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr4.bit(1u, 1), 1);
	BOOST_CHECK(!binstr1.equal(binstr4, 0, 0, 3));
}

BOOST_AUTO_TEST_CASE(set_001)
{
	GABinaryString binstr1(3);
	BOOST_CHECK_EQUAL(binstr1.size(), 3);

	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1); // higher than 1 is converted to one

	GABinaryString binstr2 = binstr1;

	// set
	binstr1.set(0, 3); // set all to 1
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u), 1);
	// TODO use other cases like only subset of binstr or over borders

	// unset
	binstr2.unset(0, 3); // set all to 0
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 0);
	// TODO use other cases like only subset of binstr or over borders
}

BOOST_AUTO_TEST_CASE(move_002)
{
	GABinaryString binstr1(8);
	BOOST_CHECK_EQUAL(binstr1.size(), 8);

	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(4u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(5u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(6u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(7u, 0), 0);

	binstr1.move(5, 1, 3);
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(4u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(5u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(6u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(7u), 0);

	// TODO test overlapping move
}

BOOST_AUTO_TEST_CASE(copy_002)
{
	GABinaryString binstr1(4);
	BOOST_CHECK_EQUAL(binstr1.size(), 4);

	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u, 1), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u, 0), 0);

	GABinaryString binstr2(0);

	binstr2.copy(binstr1, 0, 0, 4);
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(3u), 0);
}

BOOST_AUTO_TEST_SUITE_END()
