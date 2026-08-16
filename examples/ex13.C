#include "ex13.hpp"

#include <cstdlib>
#include <cstring>

int main(int argc, char **argv)
{
	unsigned int seed = 0;
	const char *filename = "smiley.txt";

	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "seed") == 0 && i + 1 < argc)
		{
			seed = static_cast<unsigned int>(std::atoi(argv[i + 1]));
			++i;
		}
		else if ((std::strcmp(argv[i], "file") == 0 || std::strcmp(argv[i], "f") == 0) &&
				 i + 1 < argc)
		{
			filename = argv[i + 1];
			++i;
		}
	}

	example13(seed, filename);
	return 0;
}
