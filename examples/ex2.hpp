#pragma once

#include <GABin2DecGenome.h>
#include <GASimpleGA.h>

#include <iostream>
#include <array>
#include <cmath>


// For this objective function we try to match the values in the array of float
// that is passed to us as userData.  If the values in the genome map to
// values that are close, we return a better score.  We are limited to positive
// values for the objective value (because we're using linear scaling - the
// default scaling method for SimpleGA), so we take the reciprocal of the
// absolute value of the difference between the value from the phenotype and
// the value in the sequence.
float objectiveEx2(GAGenome &g)
{
	GABin2DecGenome &genome = (GABin2DecGenome &)g;
	float *sequence = (float *)g.userData();

	float value = genome.nPhenotypes();
	for (int i = 0; i < genome.nPhenotypes(); i++)
		value += 1.0 / (1.0 + fabs(genome.phenotype(i) - sequence[i]));
	return value;
}

GABin2DecGenome ex2(unsigned int seed, bool useStatic)
{
	// Generate a sequence of random numbers using the values in the min and max
	// arrays.  We also set one of them to integer value to show how you can get
	// explicit integer representations by choosing your number of bits
	// appropriately.

	std::array<float, 7> target;
	std::array<float, 7> min = { 0, 0, 3, -5, 100, 0.001, 0 };
	std::array<float, 7> max = { 1, 100, 3, -2, 100000, 0.010, 7 };

	if (useStatic)
	{
		// for reproducable unit tests
		target[0] = 0.1;
		target[1] = 22;
		target[2] = 3;
		target[3] = -3.3;
		target[4] = 4243;
		target[5] = 0.002;
		target[6] = 6.6;
		std::cout << "use static input sequence (only for unit tests)" << std::endl;
	}
	else
	{
		int i = 0;
		for (auto &val : target)
		{
			val = GARandomFloat(min[i], max[i]);
			i++;
		}
		target[6] = GARandomInt((int)min[6], (int)max[6]);
	}
	// Print out the sequence to see what we got.
	std::cout << "input sequence:\n";
	for (const auto &val : target)
	{
		std::cout.width(10);
		std::cout << val << " ";
	}
	std::cout << "\n";
	std::cout.flush();

	// Create a phenotype then fill it with the phenotypes we will need to map
	// to the values we read from the file.  The arguments to the add() method
	// of a Bin2Dec phenotype are (1) number of bits, (2) min value, and (3) max
	// value. The phenotype maps a floating-point number onto the number of bits
	// that you designate.  Here we just make everything use 8 bits and use the
	// max and min that were used to generate the target values.  You can
	// experiment with the number of bits and max/min values in order to make
	// the GA work better or worse.
	GABin2DecPhenotype map;
	for (int i = 0; i < 7; i++)
	{
		map.add(8, min[i], max[i]);
	}

	// Create the template genome using the phenotype map we just made.  The
	// GA will use this genome to clone the population that it uses to do the
	// evolution.  We pass the objective function to create the genome.  We
	// also use the user data function in the genome to keep track of our
	// target values.
	GABin2DecGenome genome(map, objectiveEx2, (void*)target.data());

	// Now create the GA using the genome, set the parameters, and run it.
	auto params = std::make_shared<GAParameterList>();
	GASimpleGA ga(genome, params);
	ga.populationSize(25);
	ga.nGenerations(100);
	ga.pMutation(0.01);
	ga.pCrossover(0.6);
	ga.scoreFilename("bog.dat");
	ga.flushFrequency(50); // dump scores to disk every 50th generation
	ga.evolve(seed);

	// Dump the results of the GA to the screen.  We print out first what a
	// random genome looks like (so we get a bit of a feel for how hard it is
	// for the GA to find the right values) then we print out the best genome
	// that the GA was able to find.
	genome.initialize();
	std::cout << "random values in the genome:\n";
	
	for (unsigned int i = 0; i < map.nPhenotypes(); i++)
	{
		std::cout.width(10);
		std::cout << genome.phenotype(i) << " ";
	}
	std::cout << "\n";

	genome = ga.statistics().bestIndividual();
	std::cout << "the ga generated:\n";
	for (unsigned int i = 0; i < map.nPhenotypes(); i++)
	{
		std::cout.width(10);
		std::cout << genome.phenotype(i) << " ";
	}
	std::cout << "\n\n";
	std::cout.flush();

	// We could print out the genome directly, like this:
	// std::cout << genome << "\n";
	std::cout << "best of generation data are in 'bog.dat'\n";

	return genome;
}
