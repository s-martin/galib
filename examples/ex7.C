/* ----------------------------------------------------------------------------
  ex7.C
  mbwall 19jan95
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example functions the same way as example 3, but this example shows
how to use some of the other member functions in the GA library.  We also do
a few more fancy things with the genome (ie use the read/write methods).
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cstdlib>
#include <ga.h>

#include "ex7.hpp"

#include <fstream>
#include <iostream>
#include <sstream>


int main(int argc, char *argv[])
{
	std::cout << "Example 7\n\n";
	std::cout << "This program reads in a data file then runs a steady-state GA \n";
	std::cout << "whose objective function tries to match the pattern of bits that\n";
	std::cout << "are in the data file.\n\n";

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

	// Set the default values of the parameters.
	auto params = std::make_shared<GAParameterList>();
	params->set(gaNpopulationSize, 50); // number of individuals in population
	params->set(gaNpCrossover, 0.8); // likelihood of doing crossover
	params->set(gaNpMutation, 0.001); // probability of mutation
	params->set(gaNnGenerations, 200); // number of generations
	params->set(gaNscoreFrequency, 20); // how often to record scores
	params->set(gaNflushFrequency, 50); // how often to flush scores to file
	params->set(gaNscoreFilename, "bog.dat");
	params->parse(argc, argv);

	std::string datafile = "smiley.txt";
	std::string parmfile = "";

	// Parse the command line for arguments.  We look for two possible arguments
	// (after the parameter list has grabbed everything it recognizes).  One is
	// the name of a data file from which to read, the other is the name of a
	// parameters file from which to read.  Notice that any parameters in the
	// parameters file will override the defaults above AND any entered on the
	// command line.

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("dfile", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				std::cerr << argv[0] << ": the data file option needs a filename.\n";
				exit(1);
			}
			else
			{
				std::ostringstream str;
				str << argv[i];
				datafile = str.str();
				continue;
			}
		}
		else if (strcmp("pfile", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				std::cerr << argv[0] << ": the parameters file option needs a filename.\n";
				exit(1);
			}
			else
			{
				std::ostringstream str;
				str << argv[i];
				parmfile = str.str();
				params->read(parmfile);
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
			std::cerr << "valid arguements include GAlib arguments plus:\n";
			std::cerr << "  dfile\tdata file from which to read (" << datafile << ")\n";
			std::cerr << "  pfile\tparameters file (" << parmfile << ")\n\n";
			std::cerr << "default parameters are:\n" << params << "\n\n";
			exit(1);
		}
	}

	ex7(params, datafile);

	return 0;
}
