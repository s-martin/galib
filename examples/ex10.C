#include "ex10.hpp"

int main(int argc, char **argv)
{
	unsigned int seed = 0;
	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp(argv[ii++], "seed") == 0)
			seed = (unsigned int)atoi(argv[ii]);
	}

	example10(seed, argc, argv);
	return 0;
}
