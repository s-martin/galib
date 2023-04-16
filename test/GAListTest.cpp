#include <boost/test/unit_test.hpp>

#include <GAList.hpp>


BOOST_AUTO_TEST_SUITE( UnitTest )

BOOST_AUTO_TEST_CASE(GAList_001)
{
	GAList<int> galist0;
	BOOST_CHECK_EQUAL(galist0.size(), 0);
	
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);
	
	// TODO add failures to improve implementation
}

BOOST_AUTO_TEST_CASE(insert_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);
}

BOOST_AUTO_TEST_CASE(GAList_copy_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

	GAList<int> galist2;
	galist2.copy(galist1);

	galist1.head();
	BOOST_CHECK_EQUAL(*galist1.current(), 0);
	for(int i = 1; i < 5; i++)
	    BOOST_CHECK_EQUAL(*galist1.next(), i);

	galist2.head();
	BOOST_CHECK_EQUAL(galist2.size(), 5);

	// TODO Check why this is necessary
	BOOST_CHECK_EQUAL(*galist2.current(), 0);
	for (int i = 1; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist2.next(), i);
}

BOOST_AUTO_TEST_CASE(insertList_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);
	
	GAList<int> galist2;
	galist2.insert(10, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 4; i++)
		galist2.insert(10 + i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist2.size(), 4);

	for (int i = 0; i < 4; i++)
		BOOST_CHECK_EQUAL(*galist2.next(), 10 + i);

	galist1.warp(3);
	galist1.insert(&galist2);
	
	BOOST_REQUIRE_EQUAL(galist1.size(), 9);
	BOOST_CHECK_EQUAL(*galist1.head(), 0);
	BOOST_CHECK_EQUAL(*galist1.next(), 1);
	BOOST_CHECK_EQUAL(*galist1.next(), 2);
	BOOST_CHECK_EQUAL(*galist1.next(), 3);
	BOOST_CHECK_EQUAL(*galist1.next(), 10);
	BOOST_CHECK_EQUAL(*galist1.next(), 11);
	BOOST_CHECK_EQUAL(*galist1.next(), 12);
	BOOST_CHECK_EQUAL(*galist1.next(), 13);
	BOOST_CHECK_EQUAL(*galist1.next(), 4);
}


BOOST_AUTO_TEST_CASE(GAList_swap_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

	galist1.swap(1, 2);
	BOOST_CHECK_EQUAL(*galist1.head(), 0);
	BOOST_CHECK_EQUAL(*galist1.next(), 2);
	BOOST_CHECK_EQUAL(*galist1.next(), 1);
	BOOST_CHECK_EQUAL(*galist1.next(), 3);
	BOOST_CHECK_EQUAL(*galist1.next(), 4);

    // failure cases
	BOOST_CHECK_EQUAL(galist1.swap(2, 2), -1);
	BOOST_CHECK_EQUAL(galist1.swap(5, 3), -1);
	BOOST_CHECK_EQUAL(galist1.swap(3, 5), -1);
}

BOOST_AUTO_TEST_CASE(destroy_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

	galist1.head();
	galist1.next();
	galist1.next();
	galist1.prev();
	galist1.destroy(); 

	BOOST_CHECK_EQUAL(*galist1.current(), 0);
	BOOST_REQUIRE_EQUAL(galist1.size(), 4);
	BOOST_CHECK_EQUAL(*galist1.head(), 0);
	BOOST_CHECK_EQUAL(*galist1.next(), 2);
	BOOST_CHECK_EQUAL(*galist1.next(), 3);
	BOOST_CHECK_EQUAL(*galist1.next(), 4);
}

BOOST_AUTO_TEST_CASE(destroy_002)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

	galist1.head();
	galist1.destroy();

	BOOST_CHECK_EQUAL(*galist1.current(), 1);
	BOOST_REQUIRE_EQUAL(galist1.size(), 4);
	BOOST_CHECK_EQUAL(*galist1.head(), 1);
	BOOST_CHECK_EQUAL(*galist1.next(), 2);
	BOOST_CHECK_EQUAL(*galist1.next(), 3);
	BOOST_CHECK_EQUAL(*galist1.next(), 4);
}

BOOST_AUTO_TEST_CASE(remove_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

	galist1.head();
	galist1.next();
	galist1.next();
	galist1.prev();
	auto removedNode = galist1.remove();

	BOOST_CHECK_EQUAL(*removedNode, 1);
	BOOST_CHECK_EQUAL(*galist1.current(), 0);
	BOOST_REQUIRE_EQUAL(galist1.size(), 4);
	BOOST_CHECK_EQUAL(*galist1.head(), 0);
	BOOST_CHECK_EQUAL(*galist1.next(), 2);
	BOOST_CHECK_EQUAL(*galist1.next(), 3);
	BOOST_CHECK_EQUAL(*galist1.next(), 4);
}

BOOST_AUTO_TEST_CASE(GAList_clone_001)
{

}

BOOST_AUTO_TEST_SUITE_END()