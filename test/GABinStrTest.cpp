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
    BOOST_CHECK(binstr1.equal(binstr2, 1, 1, 2));
    BOOST_CHECK(!binstr1.equal(binstr2, 0, 1, 2));

	GABinaryString binstr3(3);
	BOOST_CHECK_EQUAL(binstr3.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr3.bit(1u, 1), 1);
	BOOST_CHECK_EQUAL(binstr3.bit(2u, 0), 0);
	BOOST_CHECK(!binstr1.equal(binstr3, 0, 0, 3));
    BOOST_CHECK(binstr1.equal(binstr3, 0, 0, 2));

	GABinaryString binstr4(2);
	BOOST_CHECK_EQUAL(binstr4.bit(0u, 0), 0);
	BOOST_CHECK_EQUAL(binstr4.bit(1u, 1), 1);
	BOOST_CHECK(!binstr1.equal(binstr4, 0, 0, 3));
    BOOST_CHECK(binstr1.equal(binstr4, 1, 1, 1));
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

	// unset
	binstr2.unset(0, 3); // set all to 0
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 0);
}

BOOST_AUTO_TEST_CASE(set_002_subset)
{
	// set/unset only a subset of the bitstring, starting at a non-zero offset.
	GABinaryString binstr1(5);
	binstr1.unset(0, 5); // start from a known all-zero state

	binstr1.set(1, 3); // set bits [1, 4): indices 1, 2 and 3
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(4u), 0);

	GABinaryString binstr2(5);
	binstr2.set(0, 5); // start from a known all-one state

	binstr2.unset(1, 3); // unset bits [1, 4): indices 1, 2 and 3
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(3u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(4u), 1);
}

BOOST_AUTO_TEST_CASE(set_003_over_border)
{
	// set/unset a range that reaches exactly up to the last valid index must not
	// touch anything beyond the bitstring, and going past the end must throw.
	GABinaryString binstr1(4);
	binstr1.unset(0, 4);

	binstr1.set(2, 2); // set bits [2, 4): indices 2 and 3 (last valid index)
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u), 1);

	BOOST_CHECK_THROW(binstr1.set(2, 3), std::out_of_range); // reaches past the end
	BOOST_CHECK_THROW(binstr1.unset(2, 3), std::out_of_range);
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
}

BOOST_AUTO_TEST_CASE(move_003_overlapping)
{
	// move() uses memmove internally, so source and destination ranges are allowed
	// to overlap; verify that an overlapping shift produces the correct result.
	GABinaryString binstr1(8);
	binstr1.bit(0u, 0);
	binstr1.bit(1u, 1);
	binstr1.bit(2u, 1);
	binstr1.bit(3u, 0);
	binstr1.bit(4u, 0);
	binstr1.bit(5u, 1);
	binstr1.bit(6u, 0);
	binstr1.bit(7u, 0);

	// shift the range [1, 7) right by one bit; destination [2, 8) overlaps source [1, 7)
	binstr1.move(2, 1, 6);
	BOOST_CHECK_EQUAL(binstr1.bit(0u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(1u), 1); // unchanged, outside the destination range
	BOOST_CHECK_EQUAL(binstr1.bit(2u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(3u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(4u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(5u), 0);
	BOOST_CHECK_EQUAL(binstr1.bit(6u), 1);
	BOOST_CHECK_EQUAL(binstr1.bit(7u), 0);

	GABinaryString binstr2(8);
	binstr2.bit(0u, 0);
	binstr2.bit(1u, 1);
	binstr2.bit(2u, 1);
	binstr2.bit(3u, 0);
	binstr2.bit(4u, 0);
	binstr2.bit(5u, 1);
	binstr2.bit(6u, 0);
	binstr2.bit(7u, 0);

	// shift the range [1, 7) left by one bit; destination [0, 6) overlaps source [1, 7)
	binstr2.move(0, 1, 6);
	BOOST_CHECK_EQUAL(binstr2.bit(0u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(1u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(2u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(3u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(4u), 1);
	BOOST_CHECK_EQUAL(binstr2.bit(5u), 0);
	BOOST_CHECK_EQUAL(binstr2.bit(6u), 0); // unchanged, outside the destination range
	BOOST_CHECK_EQUAL(binstr2.bit(7u), 0);
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
