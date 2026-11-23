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
		galist1.insert(i);

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);
}

BOOST_AUTO_TEST_CASE(GAList_insert_002)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

    GAList<int> galist2;
	galist2.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 7; i++)
		galist2.insert(i);

	BOOST_CHECK_EQUAL(galist2.size(), 7);

	for (int i = 0; i < 7; i++)
		BOOST_CHECK_EQUAL(*galist2.next(), i);
    
    BOOST_CHECK_EQUAL(galist1.insert(&galist2), GAListBASE::NO_ERR);
    BOOST_CHECK_EQUAL(galist1.size(), 12);

    BOOST_CHECK_EQUAL(*galist1.head(), 0);
	BOOST_CHECK_EQUAL(*galist1.next(), 1);
    BOOST_CHECK_EQUAL(*galist1.next(), 2);
    BOOST_CHECK_EQUAL(*galist1.next(), 3);
    BOOST_CHECK_EQUAL(*galist1.next(), 4);
    BOOST_CHECK_EQUAL(*galist1.next(), 0);
    BOOST_CHECK_EQUAL(*galist1.next(), 1);
    BOOST_CHECK_EQUAL(*galist1.next(), 2);
    BOOST_CHECK_EQUAL(*galist1.next(), 3);
    BOOST_CHECK_EQUAL(*galist1.next(), 4);
    BOOST_CHECK_EQUAL(*galist1.next(), 5);
    BOOST_CHECK_EQUAL(*galist1.next(), 6);

    // failure case
    BOOST_CHECK_EQUAL(galist2.size(), 0); // the contents were moved into galist1
    BOOST_CHECK_EQUAL(galist2.insert(&galist2), GAListBASE::ERR);
    BOOST_CHECK_EQUAL(galist2.size(), 0);
}


BOOST_AUTO_TEST_CASE(GAList_copy_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);

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
		galist1.insert(i);

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);
	
	GAList<int> galist2;
	galist2.insert(10, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 4; i++)
		galist2.insert(10 + i);

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
		galist1.insert(i);

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
	BOOST_CHECK_EQUAL(galist1.swap(2, 2), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(galist1.swap(5, 3), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(galist1.swap(3, 5), GAListBASE::NO_ERR);
}

BOOST_AUTO_TEST_CASE(destroy_001)
{
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);

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
		galist1.insert(i);

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
		galist1.insert(i);

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
	GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

    auto galist2 = galist1.clone();
   	BOOST_CHECK_EQUAL(galist2->size(), 5);

    	// TODO Check why this is necessary, see also copy
	BOOST_CHECK_EQUAL(*galist2->head(), 0);
	for (int i = 1; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist2->next(), i);
}

BOOST_AUTO_TEST_CASE(GAList_warp_001)
{
		GAList<int> galist1;
	galist1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		galist1.insert(i);

	BOOST_CHECK_EQUAL(galist1.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*galist1.next(), i);

    BOOST_CHECK_EQUAL(*galist1.current(), 4);
    galist1.warp(2);
    BOOST_CHECK_EQUAL(*galist1.current(), 2);
    galist1.warp(0);
    BOOST_CHECK_EQUAL(*galist1.current(), 0);

    // failure cases
    BOOST_CHECK_EQUAL(galist1.warp(5), nullptr);
    BOOST_CHECK_EQUAL(*galist1.current(), 0);
}

namespace
{
void fill(GAList<int> &l, int n)
{
	l.insert(0, GAListBASE::HEAD);
	for (int i = 1; i < n; i++)
		l.insert(i);
}
} // namespace

BOOST_AUTO_TEST_CASE(GAList_empty_001)
{
	GAList<int> l;
	BOOST_CHECK_EQUAL(l.size(), 0);
	BOOST_CHECK(l.current() == nullptr);
	BOOST_CHECK(l.head() == nullptr);
	BOOST_CHECK(l.tail() == nullptr);
	BOOST_CHECK(l.next() == nullptr);
	BOOST_CHECK(l.prev() == nullptr);
	BOOST_CHECK(l.warp(0) == nullptr);
	BOOST_CHECK(l.remove() == nullptr);
	BOOST_CHECK_EQUAL(l.destroy(), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(l.swap(0, 1), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(l.size(), 0);

	auto clone = l.clone();
	BOOST_CHECK_EQUAL(clone->size(), 0);
	delete clone;
}

BOOST_AUTO_TEST_CASE(GAList_single_001)
{
	GAList<int> l;
	l.insert(7, GAListBASE::HEAD);
	BOOST_CHECK_EQUAL(l.size(), 1);
	BOOST_CHECK_EQUAL(*l.head(), 7);
	BOOST_CHECK_EQUAL(*l.tail(), 7);
	BOOST_CHECK_EQUAL(*l.next(), 7); // the list is circular
	BOOST_CHECK_EQUAL(*l.prev(), 7);
	BOOST_CHECK_EQUAL(*l.current(), 7);

	BOOST_CHECK_EQUAL(l.destroy(), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(l.size(), 0);
	BOOST_CHECK(l.current() == nullptr);

	l.insert(8, GAListBASE::HEAD);
	int *removed = l.remove();
	BOOST_REQUIRE(removed != nullptr);
	BOOST_CHECK_EQUAL(*removed, 8);
	delete removed;
	BOOST_CHECK_EQUAL(l.size(), 0);
	BOOST_CHECK(l.current() == nullptr);
}

BOOST_AUTO_TEST_CASE(GAList_iteration_bounds_001)
{
	GAList<int> l;
	fill(l, 3);

	BOOST_CHECK_EQUAL(*l.head(), 0);
	BOOST_CHECK_EQUAL(*l.prev(), 2); // the list is circular: prev() at the head wraps to the tail

	BOOST_CHECK_EQUAL(*l.tail(), 2);
	BOOST_CHECK_EQUAL(*l.next(), 0); // next() at the tail wraps to the head

	BOOST_CHECK_EQUAL(*l.tail(), 2);
	BOOST_CHECK_EQUAL(*l.prev(), 1);
	BOOST_CHECK_EQUAL(*l.prev(), 0);
}

BOOST_AUTO_TEST_CASE(GAList_insert_locations_001)
{
	GAList<int> l;
	l.insert(2, GAListBASE::HEAD);
	l.insert(3, GAListBASE::AFTER);  // 2 3
	l.insert(0, GAListBASE::HEAD);   // 0 2 3
	l.insert(4, GAListBASE::TAIL);   // 0 2 3 4
	l.warp(1);
	l.insert(1, GAListBASE::BEFORE); // 0 1 2 3 4

	BOOST_REQUIRE_EQUAL(l.size(), 5);
	BOOST_CHECK_EQUAL(*l.current(), 1); // iterator moves to the inserted node
	BOOST_CHECK_EQUAL(*l.head(), 0);
	for (int i = 1; i < 5; i++)
		BOOST_CHECK_EQUAL(*l.next(), i);
	BOOST_CHECK_EQUAL(*l.tail(), 4);
}

BOOST_AUTO_TEST_CASE(GAList_insertList_locations_001)
{
	GAList<int> l;
	fill(l, 3); // 0 1 2
	GAList<int> other;
	other.insert(10, GAListBASE::HEAD);
	other.insert(11);

	l.warp(0);
	BOOST_CHECK_EQUAL(l.insert(&other, GAListBASE::BEFORE), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(other.size(), 0);
	BOOST_REQUIRE_EQUAL(l.size(), 5);
	// BEFORE the head inserts at the end of the circular list; the head does not change
	BOOST_CHECK_EQUAL(*l.head(), 0);
	BOOST_CHECK_EQUAL(*l.next(), 1);
	BOOST_CHECK_EQUAL(*l.next(), 2);
	BOOST_CHECK_EQUAL(*l.next(), 10);
	BOOST_CHECK_EQUAL(*l.next(), 11);

	// inserting a list into itself is rejected
	BOOST_CHECK_EQUAL(l.insert(&l), GAListBASE::ERR);
	BOOST_CHECK_EQUAL(l.size(), 5);
}

BOOST_AUTO_TEST_CASE(GAList_destroy_tail_001)
{
	GAList<int> l;
	fill(l, 4);
	l.tail();
	BOOST_CHECK_EQUAL(l.destroy(), GAListBASE::NO_ERR);
	BOOST_REQUIRE_EQUAL(l.size(), 3);
	BOOST_CHECK_EQUAL(*l.current(), 2); // iterator moves toward the head
	BOOST_CHECK_EQUAL(*l.tail(), 2);
}

BOOST_AUTO_TEST_CASE(GAList_remove_tail_001)
{
	GAList<int> l;
	fill(l, 4);
	l.tail();
	int *removed = l.remove();
	BOOST_REQUIRE(removed != nullptr);
	BOOST_CHECK_EQUAL(*removed, 3);
	delete removed;
	BOOST_REQUIRE_EQUAL(l.size(), 3);
	BOOST_CHECK_EQUAL(*l.current(), 2);
	BOOST_CHECK_EQUAL(*l.tail(), 2);
}

BOOST_AUTO_TEST_CASE(GAList_remove_head_001)
{
	GAList<int> l;
	fill(l, 3);
	l.head();
	int *removed = l.remove();
	BOOST_REQUIRE(removed != nullptr);
	BOOST_CHECK_EQUAL(*removed, 0);
	delete removed;
	BOOST_REQUIRE_EQUAL(l.size(), 2);
	BOOST_CHECK_EQUAL(*l.head(), 1);
	BOOST_CHECK_EQUAL(*l.next(), 2);
}

BOOST_AUTO_TEST_CASE(GAList_swap_ends_001)
{
	GAList<int> l;
	fill(l, 5);
	l.warp(2);

	BOOST_CHECK_EQUAL(l.swap(0, 4), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(l.size(), 5);
	BOOST_CHECK_EQUAL(*l.current(), 2); // swap does not move the iterator
	BOOST_CHECK_EQUAL(*l.head(), 4);
	BOOST_CHECK_EQUAL(*l.next(), 1);
	BOOST_CHECK_EQUAL(*l.next(), 2);
	BOOST_CHECK_EQUAL(*l.next(), 3);
	BOOST_CHECK_EQUAL(*l.next(), 0);
	BOOST_CHECK_EQUAL(*l.tail(), 0);
	BOOST_CHECK_EQUAL(*l.prev(), 3);
}

BOOST_AUTO_TEST_CASE(GAList_swap_adjacent_001)
{
	GAList<int> l;
	fill(l, 4);
	BOOST_CHECK_EQUAL(l.swap(2, 3), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(*l.head(), 0);
	BOOST_CHECK_EQUAL(*l.next(), 1);
	BOOST_CHECK_EQUAL(*l.next(), 3);
	BOOST_CHECK_EQUAL(*l.next(), 2);
	BOOST_CHECK_EQUAL(*l.prev(), 3);
	BOOST_CHECK_EQUAL(*l.prev(), 1);

	BOOST_CHECK_EQUAL(l.swap(0, 1), GAListBASE::NO_ERR);
	BOOST_CHECK_EQUAL(*l.head(), 1);
	BOOST_CHECK_EQUAL(*l.next(), 0);
}

BOOST_AUTO_TEST_CASE(GAList_clone_partial_001)
{
	GAList<int> l;
	fill(l, 5);
	l.warp(3);

	auto *c = l.clone(2);
	BOOST_REQUIRE_EQUAL(c->size(), 3);
	BOOST_CHECK_EQUAL(*c->head(), 2);
	BOOST_CHECK_EQUAL(*c->next(), 3);
	BOOST_CHECK_EQUAL(*c->next(), 4);
	BOOST_CHECK_EQUAL(l.size(), 5);
	BOOST_CHECK_EQUAL(*l.current(), 3); // clone does not move the original iterator

	// the clone is independent of the original
	c->head();
	c->destroy();
	BOOST_CHECK_EQUAL(c->size(), 2);
	BOOST_CHECK_EQUAL(l.size(), 5);
	delete c;

	auto *out_of_range = l.clone(10);
	BOOST_CHECK_EQUAL(out_of_range->size(), 0);
	delete out_of_range;
}

BOOST_AUTO_TEST_CASE(GAList_copy_assign_001)
{
	GAList<int> a;
	fill(a, 3);
	GAList<int> b;
	b.insert(99, GAListBASE::HEAD);
	b = a; // replaces the previous contents

	BOOST_REQUIRE_EQUAL(b.size(), 3);
	BOOST_CHECK_EQUAL(*b.head(), 0);
	BOOST_CHECK_EQUAL(*b.next(), 1);
	BOOST_CHECK_EQUAL(*b.next(), 2);

	a.head();
	a.destroy();
	BOOST_CHECK_EQUAL(a.size(), 2);
	BOOST_CHECK_EQUAL(b.size(), 3);
}

BOOST_AUTO_TEST_CASE(GAList_operator_index_001)
{
	GAList<int> l;
	fill(l, 4);
	BOOST_CHECK_EQUAL(*l[0], 0);
	BOOST_CHECK_EQUAL(*l[3], 3);
	BOOST_CHECK_EQUAL(*l.current(), 3);
	BOOST_CHECK(l[4] == nullptr);
	BOOST_CHECK_EQUAL(*l.current(), 0); // a failed warp leaves the iterator at the head
}

BOOST_AUTO_TEST_CASE(GAListIter_001)
{
	GAList<int> l;
	fill(l, 4);

	GAListIter<int> it(l);
	BOOST_CHECK_EQUAL(*it.head(), 0);
	BOOST_CHECK_EQUAL(*it.next(), 1);
	BOOST_CHECK_EQUAL(*it.tail(), 3);
	BOOST_CHECK_EQUAL(*it.next(), 0); // wraps around
	BOOST_CHECK_EQUAL(*it.prev(), 3);
	BOOST_CHECK_EQUAL(*it.prev(), 2);

	// iterators are independent of the list's embedded iterator and of each other
	GAListIter<int> copy(it);
	BOOST_CHECK_EQUAL(*copy.current(), 2);
	copy.head();
	BOOST_CHECK_EQUAL(*it.current(), 2);
	BOOST_CHECK_EQUAL(*l.current(), 3);

	BOOST_CHECK_EQUAL(*it.warp(1), 1);
	BOOST_CHECK(it.warp(4) == nullptr);
	BOOST_CHECK_EQUAL(*it.current(), 0); // a failed warp leaves the iterator at the head
}

BOOST_AUTO_TEST_SUITE_END()
