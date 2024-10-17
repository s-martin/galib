/* ----------------------------------------------------------------------------
  ex18.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SimpleGA class and 2DBinaryStringGenome class.
This program reads in a 2D pattern from a data file then tries to match the
pattern in a 2D binary string genome.  The type of GA can be specified at
the command line.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cstdlib>
#include <ga.h>
#include "ex18.hpp"

#include <iostream>
#include <fstream>

float objective(GAGenome &);
int cntr=0;

int main(int argc, char *argv[])
{
  std::cout << "Example 18\n\n";
  std::cout << "This program is designed to compare the GA types.  You can\n";
  std::cout << "specify steady-state, incremental, or simple GA and tweak any\n";
  std::cout << "of the parameters for each of these GA types.  The objective\n";
  std::cout << "function tries to match a pattern read in from a file.\n\n";
  std::cout.flush();

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GAParameterList params;
  GASimpleGA::registerDefaultParameters(params);
  GASteadyStateGA::registerDefaultParameters(params);
  GAIncrementalGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);
  params.set(gaNpCrossover, 0.9);
  params.set(gaNpMutation, 0.001);
  params.set(gaNnGenerations, 400);
  params.set(gaNpReplacement, 0.25);
  params.set(gaNscoreFrequency, 10);
  params.set(gaNflushFrequency, 50);
  params.set(gaNscoreFilename, "bog.dat");
  params.parse(argc, argv, false);

  const int SIMPLE=0, STEADY_STATE=1, INCREMENTAL=2;
  int whichGA = SIMPLE;
  int i,j;
  char filename[128] = "smiley.txt";

  for(i=1; i<argc; i++){
    if(strcmp("ga", argv[i]) == 0){
      if(++i >= argc){
         std::cerr << argv[0] << ": which GA do you want? (simple, ss, or inc)\n";
        exit(1);
      }
      else{
	if(strcmp(argv[i],"simple") == 0)
	  whichGA = SIMPLE;
	else if(strcmp(argv[i],"ss") == 0)
	  whichGA = STEADY_STATE;
	else if(strcmp(argv[i],"inc") == 0)
	  whichGA = INCREMENTAL;
	else
	   std::cerr << argv[0] << ": ga must be one of: simple, ss, or inc.\n";
        continue;
      }
    }
    else if(strcmp("file", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
         std::cerr << argv[0] << ": the file option needs a filename.\n";
        exit(1);
      }
      else{
	sprintf(filename, "%s", argv[i]);
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
       std::cerr << "  f\tfilename from which to read (" << filename << ")\n";
       std::cerr << "\n";
       std::cerr << "  ga simple|ss|inc\twhich GA to use (simple)\n";
       std::cerr << "\n";
      exit(1);
    }
  }

   std::ifstream inStream(filename);
  if(!inStream){
     std::cerr << "Cannot open " << filename << " for input.\n";
    exit(1);
  }

  int height, width;
  inStream >> height >> width;

  auto **target = new short*[width];
  for(i=0; i<width; i++)
    target[i] = new short[height];

  for(j=0; j<height; j++)
    for(i=0; i<width; i++)
      inStream >> target[i][j];

  inStream.close();

  std::cout << "input pattern:\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      std::cout << (target[i][j] == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

  GAStatistics stats = example18(params, seed, target, whichGA);

  std::cout << "the ga generated:\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
      std::cout << (genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\nthe statistics for the run are:\n" << stats;
  std::cout << "\nthe objective function was called " << cntr << " times\n";
  std::cout << "\nbest of generation data are in 'bog.dat'\n";
  std::cout.flush();

  for(i=0; i<width; i++)
    delete target[i];
  delete [] target;

  return 0;
}

float objective(GAGenome & c)
{
  auto & genome = (GA2DBinaryStringGenome &)c;
  auto **pattern = (short **)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; j<genome.height(); j++)
      value += (float)(genome.gene(i,j) == pattern[i][j]);

  cntr++;

  return(value);
}
