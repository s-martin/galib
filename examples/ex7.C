#include "ex7.hpp"

#include <cstdlib>
#include <cstring>

int main(int argc, char **argv)
{
	unsigned int seed = 0;
	bool haveSeed = false;
	std::string filename = "smiley.txt";

	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "seed") == 0 && i + 1 < argc)
		{
			seed = static_cast<unsigned int>(std::atoi(argv[i + 1]));
			haveSeed = true;
			++i;
		}
		else if ((std::strcmp(argv[i], "file") == 0 || std::strcmp(argv[i], "f") == 0) &&
				 i + 1 < argc)
		{
			filename = argv[i + 1];
			++i;
		}
	}

	if (haveSeed)
	{
		GARandomSeed(seed);
	}

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNpCrossover, 0.8);
	params.set(gaNpMutation, 0.001);
	params.set(gaNflushFrequency, 50);
	params.set(gaNscoreFilename, "bog.dat");
	params.parse(argc, argv, false);

	example7(params, filename);
	return 0;
}
