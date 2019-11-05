#include <boost/test/unit_test.hpp>

#include "ex1.hpp"

#include <ga.h>

#include <fstream>
#include <iostream>

// This objective function just tries to match the genome to the pattern in the
// user data.
float objectiveEx7(GAGenome &c)
{
	GA2DBinaryStringGenome &genome = (GA2DBinaryStringGenome &)c;
	GA2DBinaryStringGenome *pattern = (GA2DBinaryStringGenome *)c.userData();

	float value = 0.0;
	for (int i = 0; i < genome.width(); i++)
		for (int j = 0; j < genome.height(); j++)
			value += (float)(genome.gene(i, j) == pattern->gene(i, j));
	return (value);
}

// This objective function tries to maximize the value of the function
//
//                  y = -(x1*x1 + x2*x2)
//
float objectiveEx9(GAGenome &c)
{
	GABin2DecGenome &genome = (GABin2DecGenome &)c;

	float y;
	y = -genome.phenotype(0) * genome.phenotype(0);
	y -= genome.phenotype(1) * genome.phenotype(1);
	return y;
}

BOOST_AUTO_TEST_SUITE(UnitTest)

BOOST_AUTO_TEST_CASE(GAex1)
{
	auto ga = ex1();

	std::stringstream str;
	str << ga.statistics().bestIndividual();
	BOOST_CHECK_EQUAL(str.str(), "0101010101\n1010101010\n0101010101\n1010101010\n0101010101\n");
}

BOOST_AUTO_TEST_CASE(GAex7)
{
	GARandomSeed(100); // so test is always the same

	// Set the default values of the parameters.
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	BOOST_REQUIRE(params.set(gaNpopulationSize, 50)); // number of individuals in population
	BOOST_REQUIRE(params.set(gaNpCrossover, 0.8)); // likelihood of doing crossover
	BOOST_REQUIRE(params.set(gaNpMutation, 0.001)); // probability of mutation
	BOOST_REQUIRE(params.set(gaNnGenerations, 200)); // number of generations
	BOOST_REQUIRE(params.set(gaNscoreFrequency, 20)); // how often to record scores
	BOOST_REQUIRE(params.set(gaNflushFrequency, 50)); // how often to flush scores to file
	BOOST_REQUIRE(params.set(gaNscoreFilename, "bog.dat"));
	// params.parse(argc, argv, false);
	char datafile[128] = "smiley.txt";

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

	GA2DBinaryStringGenome genome(width, height, objectiveEx7, (void *)&target);
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

	std::cout << "best of generation data are in '" << ga.scoreFilename()
			  << "'\n";

	BOOST_CHECK_EQUAL(ga.statistics().maxEver(), 217);
	BOOST_CHECK_EQUAL(ga.statistics().minEver(), 97);
	BOOST_CHECK_EQUAL(ga.statistics().generation(), 160);
}

BOOST_AUTO_TEST_CASE(GAex9)
{
	// Declare variables for the GA parameters and set them to some default
	// values.
	int popsize = 30;
	int ngen = 100;
	float pmut = 0.01;
	float pcross = 0.6;

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
	ga.populationSize(popsize);
	ga.nGenerations(ngen);
	ga.pMutation(pmut);
	ga.pCrossover(pcross);
	ga.scaling(scaling);
	ga.scoreFilename("bog.dat");
	ga.scoreFrequency(10);
	ga.flushFrequency(50);
	ga.evolve(100); // static seed, so test is always the same

	// Dump the results of the GA to the screen.

	genome = ga.statistics().bestIndividual();
	std::cout << "the ga found an optimum at the point (";
	std::cout << genome.phenotype(0) << ", " << genome.phenotype(1) << ")\n\n";
	std::cout << "best of generation data are in '" << ga.scoreFilename()
			  << "'\n";

	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(0), -0.0025177002, 0.0000001);
	BOOST_CHECK_CLOSE_FRACTION(genome.phenotype(1), -0.000839233398, 0.0000001);
}

BOOST_AUTO_TEST_SUITE_END()