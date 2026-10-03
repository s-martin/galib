#include "ex13.hpp"

int main(int argc, char **argv)
{
  unsigned int seed = 0;
  const char *filename = "smiley.txt";
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii], "seed") == 0 && ii + 1 < argc)
      seed = (unsigned int)atoi(argv[++ii]);
    else if (strcmp(argv[ii], "file") == 0 && ii + 1 < argc)
      filename = argv[++ii];
  }

  example13(seed, filename);
  return 0;
}
