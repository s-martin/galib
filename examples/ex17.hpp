#pragma once

#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <iostream>

float objective(GAGenome &c)
{
	auto &genome = (GA2DArrayGenome<int> &)c;
	float score = 0.0;

	for (int i = 0; i < genome.width(); i++)
	{
		for (int j = 0; j < genome.height(); j++)
		{
			if ((i == 0 || i == genome.width() - 1) && (j == 0 || j == genome.height() - 1))
			{
				if (genome.gene(i, j) == -1)
					score += 1.0;
			}
			else if ((i + j) % 2 == 0)
			{
				if (genome.gene(i, j) == 0)
					score += 1.0;
			}
			else
			{
				if (genome.gene(i, j) == 1)
					score += 1.0;
			}
		}
	}

	return score;
}

GAStatistics example17(unsigned int seed, int argc, char **argv)
{
	std::cout << "Example 17\n\n";
	std::cout << "This program illustrates the use of a 2DArrayGenome with\n";
	std::cout
		<< "three alleles.  It tries to fill a 2D array with alternating\n";
	std::cout
		<< "0s and 1s, and -1s at the corners.  You will have to run it\n";
	std::cout
		<< "for something like 10000 generations to get the perfect score.\n\n";
	std::cout.flush();

	GA2DArrayGenome<int> genome(10, 10, objective);
	GASimpleGA ga(genome);
	ga.populationSize(30);
	ga.nGenerations(10000);
	ga.pMutation(0.01);
	ga.pCrossover(0.9);
	ga.scoreFilename("bog.dat");
	ga.flushFrequency(50);
	ga.selectScores(GAStatistics::AllScores);
	ga.evolve(seed);

	return ga.statistics();
}
