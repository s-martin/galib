#include "ex12.hpp"

#include <cstdlib>
#include <cstring>

int main(int argc, char **argv)
{
	unsigned int seed = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "seed") == 0 && i + 1 < argc)
		{
			seed = static_cast<unsigned int>(std::atoi(argv[i + 1]));
			++i;
		}
	}

	example12(seed, argc, argv);
	return 0;
}
