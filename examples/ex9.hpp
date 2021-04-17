#pragma once

#include <GABin2DecGenome.h>
#include <GASimpleGA.h>

#include <iostream>


// This objective function tries to maximize the value of the function
//
//                  y = -(x1*x1 + x2*x2)
//
float objectiveEx9(GAGenome& c)
{
	auto& genome = (GABin2DecGenome&)c;

	float y;
	y = -genome.phenotype(0) * genome.phenotype(0);
	y -= genome.phenotype(1) * genome.phenotype(1);
	return y;
}

GABin2DecGenome ex9(unsigned int seed)
{
	// Create a phenotype for two variables.  The number of bits you can use to
	// represent any number is limited by the type of computer you are using. In
	// this case, we use 16 bits to represent a floating point number whose
	// value can range from -5 to 5, inclusive.  The bounds on x1 and x2 can be
	// applied here and/or in the objective function.
	GABin2DecPhenotype map;
	map.add(16, -5, 5);
	map.add(16, -5, 5);

	// Create the template genome using the phenotype map we just made.
	GABin2DecGenome genome(map, objectiveEx9);

	// Now create the GA using the genome and run it.  We'll use sigma
	// truncation scaling so that we can handle negative objective scores.
	GASimpleGA ga(genome);
	GASigmaTruncationScaling scaling;
	ga.populationSize(30);
	ga.nGenerations(100);
	ga.pMutation(0.01);
	ga.pCrossover(0.6);
	ga.scaling(scaling);
	ga.scoreFilename("bog.dat");
	ga.scoreFrequency(10);
	ga.flushFrequency(50);
	ga.evolve(seed);

	// Dump the results of the GA to the screen.
	genome = ga.statistics().bestIndividual();
	std::cout << "the ga found an optimum at the point (";
	std::cout << genome.phenotype(0) << ", " << genome.phenotype(1) << ")\n\n";
	std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

	return genome;
}