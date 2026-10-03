#include "ex7.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Example 7\n\n";
	std::cout << "This program reads in a data file then runs a steady-state GA \n";
	std::cout << "whose objective function tries to match the pattern of bits that\n";
	std::cout << "are in the data file.\n\n";

	// Set the default values of the parameters and declare the params variable.
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNpopulationSize, 30);
	params.set(gaNpCrossover, 0.9);
	params.set(gaNpMutation, 0.001);
	params.set(gaNnGenerations, 400);
	params.set(gaNpReplacement, 0.5);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	params.parse(argc, argv, false);

	std::string datafile = "smiley.txt";
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("seed", argv[i]) == 0)
		{
			if (++i < argc)
				GARandomSeed((unsigned int)atoi(argv[i]));
		}
		else if (strcmp("file", argv[i]) == 0 || strcmp("f", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				std::cerr << argv[0] << ": the file option needs a filename.\n";
				return 1;
			}
			datafile = argv[i];
		}
	}

	example7(params, datafile);

	return 0;
}
