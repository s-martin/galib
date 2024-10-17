#include "ex3.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Example 3\n\n";
	std::cout << "This program reads in a data file then runs a simple GA whose\n";
	std::cout << "objective function tries to match the pattern of bits that are\n";
	std::cout << "in the data file.\n\n";

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.
	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp(argv[ii++], "seed") == 0)
		{
			GARandomSeed((unsigned int)atoi(argv[ii]));
		}
	}

	//   Set the default values of the parameters and declare the params
	//   variable.
	// We use the genetic algorithm's configure member to set up the parameter
	// list so that it will parse for the appropriate arguments.  Notice that
	// the params argument 'removes' from the argv list any arguments that it
	// recognized (actually it just re-orders them and changes the value of
	// argc).
	//   Once the GA's parameters are registered, we set some values that we
	//   want
	// that are different than the GAlib defaults.  Then we parse the command
	// line.
	GAParameterList params;
	GASimpleGA::registerDefaultParameters(params);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNflushFrequency, 50);
	params.set(gaNpMutation, 0.001);
	params.set(gaNpCrossover, 0.8);
	params.parse(argc, argv, false);

	std::string filename = "smiley.txt";

	// Parse the command line for arguments.
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("file", argv[i]) == 0 || strcmp("f", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				std::cerr << argv[0] << ": the file option needs a filename.\n";
				exit(1);
			}
			else
			{
				std::stringstream str;
				str << argv[i];
				filename = str.str();
				continue;
			}
		}
		else if (strcmp("seed", argv[i]) == 0)
		{
			if (++i < argc)
				continue;
			continue;
		}
		else
		{
			std::cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
			std::cerr << "valid arguments include standard GAlib arguments plus:\n";
			std::cerr << "  f\tfilename from which to read (" << filename << ")\n";
			std::cerr << "\n";
			exit(1);
		}
	}

	ex3(params, filename);

	return 0;
}
