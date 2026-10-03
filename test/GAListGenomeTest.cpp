#include <boost/test/unit_test.hpp>

#include <GAListGenome.hpp>
#include <garandom.h>


BOOST_AUTO_TEST_SUITE( UnitTest )

float objective(GAGenome &c)
{
	auto &genome = (GAListGenome<int> &)c;
	int count = 0;
	if (!genome.head())
		return 0;
	count = (*genome.head() == 1) ? 1 : 0; // move to head of the list
	for (int i = 1; i < genome.size(); i++)
		count +=
			(*genome.next() == 1) ? 1 : 0; // check each element of the list
	return 5 * count - genome.size();
}

BOOST_AUTO_TEST_CASE(GAListGenome_DestructiveMutator_001)
{
    GAListGenome<int> genome(objective);

    genome.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genome.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(genome.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*genome.next(), i);
    
    GARandomSeed(201);
    genome.mutator(GAListGenome<int>::DestructiveMutator);

    BOOST_CHECK_EQUAL(genome.mutate(-1), 0); // failure case

    BOOST_CHECK_EQUAL(genome.mutate(0.5), 2);

    BOOST_CHECK_EQUAL(genome.size(), 2);
    BOOST_CHECK_EQUAL(*genome.head(), 3);
    BOOST_CHECK_EQUAL(*genome.next(), 4);
}

BOOST_AUTO_TEST_CASE(GAListGenome_SwapMutator_001)
{
    GAListGenome<int> genome(objective);

    genome.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genome.insert(i);		// each subsequent node contains a number

	BOOST_CHECK_EQUAL(genome.size(), 5);

	for (int i = 0; i < 5; i++)
		BOOST_CHECK_EQUAL(*genome.next(), i);
    
    GARandomSeed(202);
    genome.mutator(GAListGenome<int>::SwapMutator);

    BOOST_CHECK_EQUAL(genome.mutate(-1), 0); // failure case

    BOOST_CHECK_EQUAL(genome.mutate(0.5), 2);

    BOOST_CHECK_EQUAL(*genome.head(), 0);
    BOOST_CHECK_EQUAL(*genome.next(), 1);
    BOOST_CHECK_EQUAL(*genome.next(), 4);
    BOOST_CHECK_EQUAL(*genome.next(), 2);
    BOOST_CHECK_EQUAL(*genome.next(), 3);
}

BOOST_AUTO_TEST_CASE(GAListGenome_NodeComparator_001)
{
    GAListGenome<int> genome1(objective);
    genome1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genome1.insert(i);		// each subsequent node contains a number

    BOOST_CHECK_EQUAL(GAListGenome<int>::NodeComparator(genome1, genome1), 0); // failure case

    GAListGenome<int> genome_failure(objective);
    genome_failure.insert(0, GAListBASE::HEAD); // the head node contains a '0'
    BOOST_CHECK_EQUAL(GAListGenome<int>::NodeComparator(genome1, genome_failure), 4); // failure case
    BOOST_CHECK_EQUAL(GAListGenome<int>::NodeComparator(genome_failure, genome1), 4); // failure case

    GAListGenome<int> genome_failure2(objective);
    BOOST_CHECK_EQUAL(GAListGenome<int>::NodeComparator(genome_failure2, genome_failure2), 0); // failure case

    GAListGenome<int> genome2(objective);
    genome2.insert(3, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genome2.insert(i+3);		// each subsequent node contains a number

    BOOST_CHECK_EQUAL(GAListGenome<int>::NodeComparator(genome1, genome2), 5);
}

BOOST_AUTO_TEST_CASE(GAListGenome_OnePointCrossover_001)
{
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);		// each subsequent node contains a number

    GAListGenome<int> genomep2(objective);
    genomep2.insert(5, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep2.insert(i+5);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    GARandomSeed(204);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OnePointCrossover(genomep1, genomep1, &genomec1, &genomec2), 2);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 5);
	BOOST_CHECK_EQUAL(*genomep2.next(), 6);
    BOOST_CHECK_EQUAL(*genomep2.next(), 7);
    BOOST_CHECK_EQUAL(*genomep2.next(), 8);
    BOOST_CHECK_EQUAL(*genomep2.next(), 9);

    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
    BOOST_CHECK_EQUAL(*genomec1.next(), 0);
    BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 3);

    BOOST_CHECK_EQUAL(*genomec2.head(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 4);
    BOOST_CHECK_EQUAL(*genomec2.next(), 1);

    BOOST_CHECK_EQUAL(GAListGenome<int>::OnePointCrossover(genomep1, genomep1, nullptr, nullptr), 0);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OnePointCrossover(genomep1, genomep1, &genomec1, nullptr), 1);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OnePointCrossover(genomep1, genomep1, nullptr, &genomec2), 1);
}

BOOST_AUTO_TEST_CASE(GAListGenome_PartialMatchCrossover_001)
{
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);		// each subsequent node contains a number

    GAListGenome<int> genomep2(objective);
    genomep2.insert(5, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep2.insert(i+5);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    GARandomSeed(205);
    BOOST_CHECK_EQUAL(GAListGenome<int>::PartialMatchCrossover(genomep1, genomep1, &genomec1, &genomec2), 3);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 5);
	BOOST_CHECK_EQUAL(*genomep2.next(), 6);
    BOOST_CHECK_EQUAL(*genomep2.next(), 7);
    BOOST_CHECK_EQUAL(*genomep2.next(), 8);
    BOOST_CHECK_EQUAL(*genomep2.next(), 9);

    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
	BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 3);
    BOOST_CHECK_EQUAL(*genomec1.next(), 4);

    BOOST_CHECK_EQUAL(*genomec2.head(), 0);
	BOOST_CHECK_EQUAL(*genomec2.next(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 4);

    BOOST_CHECK_EQUAL(GAListGenome<int>::PartialMatchCrossover(genomep1, genomep1, nullptr, nullptr), 0);
    BOOST_CHECK_EQUAL(GAListGenome<int>::PartialMatchCrossover(genomep1, genomep1, &genomec1, nullptr), 1);
    BOOST_CHECK_EQUAL(GAListGenome<int>::PartialMatchCrossover(genomep1, genomep1, nullptr, &genomec2), 2);

    GAListGenome<int> genome_failure(objective);
    genome_failure.insert(0, GAListBASE::HEAD);
    BOOST_CHECK_EQUAL(GAListGenome<int>::PartialMatchCrossover(genomep1, genome_failure, &genomec1, &genomec2), 0);
}

BOOST_AUTO_TEST_CASE(GAListGenome_OrderCrossover_001)
{
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);		// each subsequent node contains a number

    GAListGenome<int> genomep2(objective);
    genomep2.insert(5, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep2.insert(i+5);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    GARandomSeed(206);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genomep1, &genomec1, &genomec2), 2);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 5);
	BOOST_CHECK_EQUAL(*genomep2.next(), 6);
    BOOST_CHECK_EQUAL(*genomep2.next(), 7);
    BOOST_CHECK_EQUAL(*genomep2.next(), 8);
    BOOST_CHECK_EQUAL(*genomep2.next(), 9);

    // Both parents passed here are the very same genome instance (genomep1), so the
    // crossover is effectively a no-op: the "child" section always matches the
    // single parent regardless of which crossover operator is used, which is why
    // the result below is indistinguishable from PartialMatchCrossover's result.
    // See GAListGenome_OrderCrossover_002 for a test that exercises OrderCrossover
    // with two genuinely different (permuted) parents.
    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
	BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 3);
    BOOST_CHECK_EQUAL(*genomec1.next(), 4);

    BOOST_CHECK_EQUAL(*genomec2.head(), 0);
	BOOST_CHECK_EQUAL(*genomec2.next(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 4);

    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genomep1, nullptr, nullptr), 0);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genomep1, &genomec1, nullptr), 1);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genomep1, nullptr, &genomec2), 1);

    GAListGenome<int> genome_failure(objective);
    genome_failure.insert(0, GAListBASE::HEAD);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genome_failure, &genomec1, &genomec2), 0);
}

BOOST_AUTO_TEST_CASE(GAListGenome_OrderCrossover_002)
{
    // OrderCrossover requires both parents to contain the same set of elements
    // (just reordered), as is typical for permutation problems (e.g. TSP). Here
    // genomep2 is a genuine reordering (reverse) of genomep1 so the operator has
    // something real to recombine, unlike in GAListGenome_OrderCrossover_001 where
    // both parents are the same instance.
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);

    GAListGenome<int> genomep2(objective);
    genomep2.insert(4, GAListBASE::HEAD);
	for (int i = 3; i >= 0; i--)
		genomep2.insert(i);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    GARandomSeed(3);
    BOOST_CHECK_EQUAL(GAListGenome<int>::OrderCrossover(genomep1, genomep2, &genomec1, &genomec2), 2);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 4);
	BOOST_CHECK_EQUAL(*genomep2.next(), 3);
    BOOST_CHECK_EQUAL(*genomep2.next(), 2);
    BOOST_CHECK_EQUAL(*genomep2.next(), 1);
    BOOST_CHECK_EQUAL(*genomep2.next(), 0);

    // the children contain a genuine recombination of both parents' orderings
    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
	BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 3);
    BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 4);

    BOOST_CHECK_EQUAL(*genomec2.head(), 4);
	BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 0);
}

BOOST_AUTO_TEST_CASE(GAListGenome_CycleCrossover_001)
{
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD); // the head node contains a '0'
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);		// each subsequent node contains a number

    GAListGenome<int> genomep2(objective);
    genomep2.insert(5, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep2.insert(i+5);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    GARandomSeed(207);
    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genomep1, &genomec1, &genomec2), 2);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 5);
	BOOST_CHECK_EQUAL(*genomep2.next(), 6);
    BOOST_CHECK_EQUAL(*genomep2.next(), 7);
    BOOST_CHECK_EQUAL(*genomep2.next(), 8);
    BOOST_CHECK_EQUAL(*genomep2.next(), 9);

    // Both parents passed here are the very same genome instance (genomep1), so the
    // crossover is effectively a no-op: the "child" section always matches the
    // single parent regardless of which crossover operator is used, which is why
    // the result below is indistinguishable from PartialMatchCrossover's result.
    // See GAListGenome_CycleCrossover_002 for a test that exercises CycleCrossover
    // with two genuinely different (permuted) parents.
    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
	BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 3);
    BOOST_CHECK_EQUAL(*genomec1.next(), 4);

    BOOST_CHECK_EQUAL(*genomec2.head(), 0);
	BOOST_CHECK_EQUAL(*genomec2.next(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 4);

    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genomep1, nullptr, nullptr), 0);
    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genomep1, &genomec1, nullptr), 1);
    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genomep1, nullptr, &genomec2), 1);

    GAListGenome<int> genome_failure(objective);
    genome_failure.insert(0, GAListBASE::HEAD);
    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genome_failure, &genomec1, &genomec2), 0);
}

BOOST_AUTO_TEST_CASE(GAListGenome_CycleCrossover_002)
{
    // CycleCrossover requires both parents to contain the same set of elements
    // (just reordered), as is typical for permutation problems (e.g. TSP), and is
    // itself deterministic (it does not use the RNG). Here genomep2 is a genuine
    // reordering (reverse) of genomep1 so the operator has something real to
    // recombine, unlike in GAListGenome_CycleCrossover_001 where both parents are
    // the same instance.
    GAListGenome<int> genomep1(objective);
    genomep1.insert(0, GAListBASE::HEAD);
	for (int i = 1; i < 5; i++)
		genomep1.insert(i);

    GAListGenome<int> genomep2(objective);
    genomep2.insert(4, GAListBASE::HEAD);
	for (int i = 3; i >= 0; i--)
		genomep2.insert(i);

    GAListGenome<int> genomec1(objective);
    GAListGenome<int> genomec2(objective);
    BOOST_CHECK_EQUAL(GAListGenome<int>::CycleCrossover(genomep1, genomep2, &genomec1, &genomec2), 2);

    BOOST_CHECK_EQUAL(*genomep1.head(), 0);
	BOOST_CHECK_EQUAL(*genomep1.next(), 1);
    BOOST_CHECK_EQUAL(*genomep1.next(), 2);
    BOOST_CHECK_EQUAL(*genomep1.next(), 3);
    BOOST_CHECK_EQUAL(*genomep1.next(), 4);

    BOOST_CHECK_EQUAL(*genomep2.head(), 4);
	BOOST_CHECK_EQUAL(*genomep2.next(), 3);
    BOOST_CHECK_EQUAL(*genomep2.next(), 2);
    BOOST_CHECK_EQUAL(*genomep2.next(), 1);
    BOOST_CHECK_EQUAL(*genomep2.next(), 0);

    // the children contain a genuine recombination of both parents' orderings
    BOOST_CHECK_EQUAL(*genomec1.head(), 0);
	BOOST_CHECK_EQUAL(*genomec1.next(), 3);
    BOOST_CHECK_EQUAL(*genomec1.next(), 2);
    BOOST_CHECK_EQUAL(*genomec1.next(), 1);
    BOOST_CHECK_EQUAL(*genomec1.next(), 4);

    BOOST_CHECK_EQUAL(*genomec2.head(), 4);
	BOOST_CHECK_EQUAL(*genomec2.next(), 1);
    BOOST_CHECK_EQUAL(*genomec2.next(), 2);
    BOOST_CHECK_EQUAL(*genomec2.next(), 3);
    BOOST_CHECK_EQUAL(*genomec2.next(), 0);
}

BOOST_AUTO_TEST_SUITE_END()