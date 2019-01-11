#include <boost/test/unit_test.hpp>

#include <GASimpleGA.h>
#include <GA2DBinStrGenome.h>
#include <std_stream.h>

// This is the objective function.  All it does is check for alternating 0s and
// 1s.  If the gene is odd and contains a 1, the fitness is incremented by 1.
// If the gene is even and contains a 0, the fitness is incremented by 1.  No
// penalties are assigned. 
//   We have to do the cast because a plain, generic GAGenome doesn't have 
// the members that a GA2DBinaryStringGenome has.  And it's ok to cast it
// because we know that we will only get GA2DBinaryStringGenomes and
// nothing else.
float Objective(GAGenome& g) 
{
    GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g;
    float score = 0.0;
    int count = 0;
    for (int i = 0; i < genome.width(); i++)
    {
        for (int j = 0; j < genome.height(); j++)
        {
            if (genome.gene(i, j) == 0 && count % 2 == 0)
            {
	            score += 1.0;
            }
            if (genome.gene(i, j) == 1 && count % 2 != 0)
	        {
                   score += 1.0;
            }
            count++;
        }
    }
    return score;
}

BOOST_AUTO_TEST_SUITE( UnitTest )

BOOST_AUTO_TEST_CASE(GASimpleGA_001)
{
    int width    = 10;
    int height   = 5;
    // Now create the GA and run it.  First we create a genome of the type that
    // we want to use in the GA.  The ga doesn't operate on this genome in the
    // optimization - it just uses it to clone a population of genomes.
    GA2DBinaryStringGenome genome(width, height, Objective);

    // Now that we have the genome, we create the genetic algorithm and set
    // its parameters - number of generations, mutation probability, and crossover
    // probability.  And finally we tell it to evolve itself.
    GASimpleGA ga(genome);
    ga.populationSize(30);
    ga.nGenerations(400);
    ga.pMutation(0.001);
    ga.pCrossover(0.9);
    ga.evolve();

    std::stringstream str;
    str << ga.statistics().bestIndividual();
    BOOST_CHECK_EQUAL(str.str(), "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n");
}

BOOST_AUTO_TEST_SUITE_END()