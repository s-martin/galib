#include <boost/test/unit_test.hpp>

#include <GA3DBinStrGenome.h>
#include <GASStateGA.h>
#include <stdio.h>

// based on example 4

// This is the objective function.  All it does is give one point for every
// odd bit that is set and one point for every even bit that is not set.
float objective(GAGenome &g)
{
	GA3DBinaryStringGenome &genome = (GA3DBinaryStringGenome &)g;
	float value = 0.0;
	int count = 0;
	int I = genome.width(), J = genome.height(), K = genome.depth();
	for (int i = 0; i < I; i++)
	{
		for (int j = 0; j < J; j++)
		{
			for (int k = 0; k < K; k++)
			{
				if (genome.gene(i, j, k) == 0 && count % 2 == 0)
					value += 1.0;
				else if (genome.gene(i, j, k) == 1 && count % 2 != 0)
					value += 1.0;
				count++;
			}
		}
	}
	return value;
}

BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GASStateGA_001)
{
	int depth = 3;
	int width = 10;
	int height = 5;

	// Now create the GA and run it.  First we create a genome of the type that
	// we want to use in the GA.  The ga doesn't use this genome in the
	// optimization - it just uses it to clone a population of genomes.
	GA3DBinaryStringGenome genome(width, height, depth, objective);

	// Now that we have the genome, we create the genetic algorithm and set
	// its parameters - number of generations, mutation probability, and
	// crossover probability.  By default the GA keeps track of the best of
	// generation scores and also keeps one genome as the 'best of all' - the
	// best genome that it encounters from all of the generations.  Here we tell
	// the GA to keep track of all scores, not just the best-of-generation.
	GASteadyStateGA ga(genome);
	ga.populationSize(100);
	ga.pReplacement(0.50); // replace 50% of population each generation
	//  ga.nReplacement(4);	          // number of individuals to replace each
	//  gen
	ga.nGenerations(200);
	ga.pMutation(0.001);
	ga.pCrossover(0.9);
	ga.scoreFilename("bog.dat"); // name of file for scores
	ga.scoreFrequency(10); // keep the scores of every 10th generation
	ga.flushFrequency(50); // specify how often to write the score to disk
	ga.selectScores(GAStatistics::AllScores);
	ga.evolve();

	std::stringstream str;
	str << ga.statistics().bestIndividual();
	BOOST_CHECK_EQUAL(
		str.str(),
        "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"
        "1010101010\n0101010101\n1010101010\n0101010101\n1010101010\n\n"
        "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n\n"        
        );
}

BOOST_AUTO_TEST_SUITE_END()