#pragma once

#include <GA2DBinStrGenome.h>
#include <GASStateGA.h>

#include <fstream>
#include <iostream>


// This objective function just tries to match the genome to the pattern in the
// user data.
float objectiveEx7(GAGenome& c)
{
	auto& genome = (GA2DBinaryStringGenome&)c;
	auto* pattern = (GA2DBinaryStringGenome*)c.userData();

	float value = 0.0;
	for (int i = 0; i < genome.width(); i++)
		for (int j = 0; j < genome.height(); j++)
			value += (float)(genome.gene(i, j) == pattern->gene(i, j));
	return (value);
}

GASteadyStateGA ex7(const std::shared_ptr<GAParameterList>& params, const std::string &datafile)
{
	// Read in the pattern from the specified file.  File format is pretty
	// simple: two integers that give the height then width of the matrix, then
	// the matrix of 1's and 0's (with whitespace inbetween).
	//   Here we use a binary string genome to store the desired pattern.  This
	// shows how you can read in directly from a stream into a genome.  (This
	// can be useful in a population initializer when you want to bias your
	// population)
	std::ifstream inStream(datafile);
	if (!inStream)
	{
		std::cerr << "Cannot open " << datafile << " for input.\n";
		exit(1);
	}

	int height, width;
	inStream >> height >> width;
	GA2DBinaryStringGenome target(width, height);
	inStream >> target;
	inStream.close();

	// Print out the pattern to be sure we got the right one.

	std::cout << "input pattern:\n";
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
			std::cout << (target.gene(i, j) == 1 ? '*' : ' ') << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout.flush();

	// Now create the first genome and the GA.  When we create the genome, we
	// give it not only the objective function but also 'user data'.  In this
	// case, the user data is a pointer to our target pattern.  From a C++ point
	// of view it would be better to derive a new genome class with its own
	// data, but here we just want a quick-and-dirty implementation, so we use
	// the user-data.

	GA2DBinaryStringGenome genome(width, height, objectiveEx7, (void*)&target);
	GASteadyStateGA ga(genome, params);

	// The default selection method is RouletteWheel.  Here we set the selection
	// method to TournamentSelection.
	GATournamentSelector selector;
	ga.selector(selector);

	// The following member functions override the values that were set using
	// the parameter list.  They are commented out here so that you can see how
	// they would be used.

	// We can control the amount of overlap from generation to generation using
	// the pReplacement member function.  If we specify a value of 1 (100%) then
	// the entire population is replaced each generation.  Notice that the
	// percentage must be high enough to have at least one individual produced
	// in each generation.  If not, the GA will post a warning message.

	//  ga.pReplacement(0.3);

	// Often we use the number of generations as the criterion for terminating
	// the GA run.  Here we override that and tell the GA to use convergence as
	// a termination criterion.  Note that you can pass ANY function as the
	// stopping criterion (as long as it has the correct signature).
	//   Notice that the values we set here for p- and n-convergence override
	//   those
	// that we set in the parameters object.

	ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);

	//  ga.pConvergence(0.99);	        // converge to within 1%
	//  ga.nConvergence(100);		// within the last 100 generations

	// Evolve the GA 'by hand'.  When you use this method, be sure to initialize
	// the GA before you start to evolve it.  You can print out the status of
	// the current population by using the ga.population() member function. This
	// is also how you would print the status of the GA to disk along the way
	// (rather than waiting to the end then printing the scores, for example).

	ga.initialize();
	while (!ga.done())
	{
		++ga;
	}
	ga.flushScores();

	// Now that the GA is finished, we set our default genome to equal the
	// contents of the best genome that the GA found.  Then we print it out.

	genome = ga.statistics().bestIndividual();
	std::cout << "the ga generated:\n";
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
			std::cout << (genome.gene(i, j) == 1 ? '*' : ' ') << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout.flush();

	std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

	return ga;
}
