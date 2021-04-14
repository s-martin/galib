#include <boost/test/unit_test.hpp>

#include <GABinStr.h>


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


	//int i1 = 1;
	//int i2 = 2;
	//gaarray[0u] = i1;
	//gaarray[1u] = i2;

	//BOOST_CHECK_EQUAL(gaarray[0u], 1);
	//BOOST_CHECK_EQUAL(gaarray[1u], 2);

	//auto gaarray2 = gaarray;
	//BOOST_CHECK_EQUAL(gaarray2.size(), 2);
	//BOOST_CHECK_EQUAL(gaarray2[0u], 1);
	//BOOST_CHECK_EQUAL(gaarray2[1u], 2);

	//BOOST_CHECK(gaarray == gaarray2);

	//GAArray gaarray3(gaarray);
	//BOOST_CHECK_EQUAL(gaarray3.size(), 2);
	//BOOST_CHECK_EQUAL(gaarray3[0u], 1);
	//BOOST_CHECK_EQUAL(gaarray3[1u], 2);

	//GAArray<int> gaarray4(2);
	//gaarray4[0u] = 3;
	//gaarray4[1u] = 4;
	//BOOST_CHECK_EQUAL(gaarray4.size(), 2);
	//BOOST_CHECK_EQUAL(gaarray4[0u], 3);
	//BOOST_CHECK_EQUAL(gaarray4[1u], 4);

	//BOOST_CHECK(gaarray != gaarray4);

	//// TODO add failures to improve implementation
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

//BOOST_AUTO_TEST_CASE(equal_001)
//{
//	GABinaryString binstr1(3);
//	BOOST_CHECK_EQUAL(binstr1.size(), 3);
//
//	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
//	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
//	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1); // higher than 1 is converted to one
//
//	GABinaryString binstr2 = binstr1;
//	BOOST_CHECK(binstr1.equal(binstr2, 0, 0, 3));
//}
//
//BOOST_AUTO_TEST_CASE(set_001)
//{
//	GABinaryString binstr1(3);
//	BOOST_CHECK_EQUAL(binstr1.size(), 3);
//
//	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 0);
//	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
//	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1); // higher than 1 is converted to one
//
//	GABinaryString binstr2 = binstr1;
//
//	// set
//	binstr1.set(0, 3); // set all to 1
//	BOOST_CHECK_EQUAL(binstr1.bit(0u, 0), 1);
//	BOOST_CHECK_EQUAL(binstr1.bit(1u, 1), 1);
//	BOOST_CHECK_EQUAL(binstr1.bit(2u, 2), 1);
//	// TODO use other cases like only subset of binstr or over borders
//
//	// unset
//	binstr2.unset(0, 3); // set all to 0
//	BOOST_CHECK_EQUAL(binstr2.bit(0u, 0), 0);
//	BOOST_CHECK_EQUAL(binstr2.bit(1u, 1), 0);
//	BOOST_CHECK_EQUAL(binstr2.bit(2u, 2), 0);
//	// TODO use other cases like only subset of binstr or over borders
//}

//BOOST_AUTO_TEST_CASE(swap_001)
//{
//	GAArray<int> gaarray5(2);
//	gaarray5[0u] = 1;
//	gaarray5[1u] = 2;
//	BOOST_CHECK_EQUAL(gaarray5[0u], 1);
//	BOOST_CHECK_EQUAL(gaarray5[1u], 2);
//
//	auto gaarray5a = gaarray5;
//	auto gaarray5b = gaarray5;
//
//	gaarray5.swap(0, 1);
//	BOOST_CHECK_EQUAL(gaarray5[0u], 2);
//	BOOST_CHECK_EQUAL(gaarray5[1u], 1);
//
//	// failures
//	BOOST_CHECK_THROW(gaarray5a.swap(0, 3), std::out_of_range);
//	BOOST_CHECK_THROW(gaarray5b.swap(3, 1), std::out_of_range);
//}
//
//BOOST_AUTO_TEST_CASE(copy_001)
//{
//	GAArray<int> gaarray6(2);
//	gaarray6[0u] = 5;
//	gaarray6[1u] = 6;
//	BOOST_CHECK_EQUAL(gaarray6[0u], 5);
//	BOOST_CHECK_EQUAL(gaarray6[1u], 6);
//
//	GAArray<int> gaarray7(2);
//	gaarray7[0u] = 1;
//	gaarray7[1u] = 2;
//
//	gaarray7.copy(gaarray6, 1, 0, 1);
//	BOOST_CHECK_EQUAL(gaarray7[0u], 1);
//	BOOST_CHECK_EQUAL(gaarray7[1u], 5);
//
//	// TODO add failures to improve implementation
//}
//
//BOOST_AUTO_TEST_CASE(size_001)
//{
//	GAArray<int> gaarray8(2);
//	gaarray8[0u] = 1;
//	gaarray8[1u] = 2;
//	BOOST_CHECK_EQUAL(gaarray8.size(), 2);
//	BOOST_CHECK_EQUAL(gaarray8[0u], 1);
//	BOOST_CHECK_EQUAL(gaarray8[1u], 2);
//
//	auto gaarray8a = gaarray8;
//
//	gaarray8.size(5);
//	BOOST_CHECK_EQUAL(gaarray8.size(), 5);
//	BOOST_CHECK_EQUAL(gaarray8[0u], 1);
//	BOOST_CHECK_EQUAL(gaarray8[1u], 2);
//
//	// TODO fix implementation
//	BOOST_CHECK_EQUAL(gaarray8[2u], 0);
//	BOOST_CHECK_EQUAL(gaarray8[3u], 0);
//	BOOST_CHECK_EQUAL(gaarray8[4u], 0);
//
//	gaarray8a.size(2);
//	BOOST_CHECK_EQUAL(gaarray8a.size(), 2);
//	BOOST_CHECK_EQUAL(gaarray8a[0u], 1);
//	BOOST_CHECK_EQUAL(gaarray8a[1u], 2);
//
//
//	// TODO add failures to improve implementation
//}
//
//BOOST_AUTO_TEST_CASE(move_001)
//{
//	GAArray<int> gaarray9(4);
//	gaarray9[0u] = 5;
//	gaarray9[1u] = 6;
//	gaarray9[2u] = 7;
//	gaarray9[3u] = 8;
//
//	auto gaarray9a = gaarray9;
//
//	gaarray9.move(0, 2, 2);
//	BOOST_CHECK_EQUAL(gaarray9[0u], 7);
//	BOOST_CHECK_EQUAL(gaarray9[1u], 8);
//	BOOST_CHECK_EQUAL(gaarray9[2u], 7);
//	BOOST_CHECK_EQUAL(gaarray9[3u], 8);
//
//	gaarray9a.move(2, 0, 2);
//	BOOST_CHECK_EQUAL(gaarray9a[0u], 5);
//	BOOST_CHECK_EQUAL(gaarray9a[1u], 6);
//	BOOST_CHECK_EQUAL(gaarray9a[2u], 5);
//	BOOST_CHECK_EQUAL(gaarray9a[3u], 6);
//
//	// TODO add failures to improve implementation
//}

BOOST_AUTO_TEST_SUITE_END()
