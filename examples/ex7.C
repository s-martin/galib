#include "ex7.hpp"

GAStatistics example7(GAParameterList params, const std::string &datafile)
{
	std::cout << "Example 7\n\n";
	std::cout << "This program reads in a data file then runs a steady-state GA \n";
	std::cout << "whose objective function tries to match the pattern of bits that\n";
	std::cout << "are in the data file.\n\n";

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
	GASteadyStateGA ga(genome);

	// When you use a GA with overlapping populations, the default score
	// frequency (how often the best of generation score is recorded) defaults
	// to 100.  We use the parameters member function to change this value
	// (along with all of the other parameters we set above).  You can also
	// change the score frequency using the scoreFrequency member function of
	// the GA.  Each of the parameters can be set individually if you like.
	//   Here we just use the values that were set in the parameter list.
	ga.parameters(params);

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

	return ga.statistics();
}
