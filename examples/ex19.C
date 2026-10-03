/* ----------------------------------------------------------------------------
  ex19.C
  mbwall 5sep95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example runs all of the DeJong functions.  You can specify which 
function you want at the command line.  We use SigmaTruncation scaling to make
certain that negative objective scores won't be a problem for us.
   The DeJong functions in this code were pinched from the eval.c file in the
pga-2.8 genetic algorithm package (under the terms of the GNU General Public
License).
---------------------------------------------------------------------------- */
#include "ex19.hpp"

 
 

int
main(int argc, char *argv[])
{
  std::cout << "Example 19\n\n";
  std::cout << "This program runs the DeJong test problems.\n\n";
  std::cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);	// population size
  params.set(gaNpCrossover, 0.9);	// probability of crossover
  params.set(gaNpMutation, 0.001);	// probability of mutation
  params.set(gaNnGenerations, 400);	// number of generations
  params.set(gaNpReplacement, 0.25);	// how much of pop to replace each gen
  params.set(gaNscoreFrequency, 10);	// how often to record scores
  params.set(gaNflushFrequency, 50);	// how often to dump scores to file
  params.set(gaNscoreFilename, "bog.dat");
  params.parse(argc, argv, false);    // parse command line for GAlib args

  int whichFunction = 0;

  for(int i=1; i<argc; i++){
    if(strcmp("function", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
         std::cerr << argv[0] << ": you must specify a function (1-5)\n";
        exit(1);
      }
      else{
	whichFunction = atoi(argv[i]) - 1;
	if(whichFunction < 0 || whichFunction > 4){
	   std::cerr << argv[0] << ": the function must be in the range [1,5]\n";
	  exit(1);
	}
        continue;
      }
    }
    else if(strcmp("seed", argv[i]) == 0){
      if(++i < argc) continue;
      continue;
    }
    else {
       std::cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
       std::cerr << "valid arguements include standard GAlib arguments plus:\n";
       std::cerr << "  f\twhich function to evaluate (all)\n";
       std::cerr << "parameters are:\n\n" << params << "\n\n";
      exit(1);
    }
  }

  std::cout << "running DeJong function number " << (whichFunction + 1) << " ...\n";

  example19(seed, whichFunction);

  return 0;
}
