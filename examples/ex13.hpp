#pragma once

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ga.h>
#include <iostream>
#include <fstream>

typedef struct _UserData {
  int width, height;
  short **picture_target;
  float *numbers_target;
  GA2DBinaryStringGenome *picture_genome;
  GABin2DecGenome *numbers_genome;
} UserData;

float PictureObjective(GAGenome &);
float NumbersObjective(GAGenome &);

float objective(GAGenome & g)
{
    return PictureObjective(g);
}

GAStatistics example13(unsigned int seed, const char* filename)
{
  std::cout << "Example 13\n\n";
  std::cout << "This program runs a GA-within-GA.  The outer level GA tries to\n";
  std::cout << "match the pattern read in from a file.  The inner level GA is\n";
  std::cout << "run only when the outer GA reaches a threshhold objective score\n";
  std::cout << "then it tries to match a sequence of numbers that were generated\n";
  std::cout << "randomly at the beginning of the program's execution.\n\n";
  std::cout << "You might have to run the primary GA for more than 5000\n";
  std::cout << "generations to get a score high enough to kick in the secondary\n";
  std::cout << "genetic algorithm.  Use the ngen option to do this on the\n";
  std::cout << "command line.\n\n";

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  GARandomSeed(seed);

// Set the default values of the parameters and declare the params variable.

  int i,j;
  GAParameterList params;
  GAIncrementalGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 150);
  params.set(gaNpCrossover, 0.8);
  params.set(gaNpMutation, 0.005);
  params.set(gaNnGenerations, 500);
  params.set(gaNscoreFilename, "bog.dat");
  params.set(gaNscoreFrequency, 10);
  params.set(gaNflushFrequency, 50);

// Create a user data object.  We'll keep all of the information for this 
// program in this object.
  UserData data;

// Read in the pattern from the specified file.  File format is pretty simple:
// two integers that give the height then width of the matrix, then the matrix
// of 1's and 0's (with whitespace inbetween).

   std::ifstream inStream(filename);
  if(!inStream){
     std::cerr << "Cannot open " << filename << " for input.\n";
    exit(1);
  }
  inStream >> data.height >> data.width;
  data.picture_target = new short*[data.width];
  for(i=0; i<data.width; i++)
    data.picture_target[i] = new short[data.height];
  for(j=0; j<data.height; j++)
    for(i=0; i<data.width; i++)
      inStream >> data.picture_target[i][j];
  inStream.close();

// Print out the pattern to be sure we got the right one.

  std::cout << "input pattern:\n";
  for(j=0; j<data.height; j++){
    for(i=0; i<data.width; i++)
      std::cout << (data.picture_target[i][j] == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

// Generate the random sequence of numbers.

  int n=7;
  float min[] = {0, 0,   3, -5, 100,    0.001, 0};
  float max[] = {1, 100, 3, -2, 100000, 0.010, 7};
  data.numbers_target = new float[n];
  for(i=0; i<n; i++)
    data.numbers_target[i] = GARandomFloat(min[i], max[i]);
  data.numbers_target[6] = GARandomInt((int)min[6], (int)max[6]);

// Print out the sequence so we'll know what we have to match.

  std::cout << "input sequence:\n";
  for(i=0; i<n; i++){
    std::cout.width(10);
    std::cout << data.numbers_target[i] << " ";
  }
  std::cout << "\n"; std::cout.flush();


// Create a phenotype for the numbers to map them to a bin2dec genome.

  GABin2DecPhenotype map;
  for(i=0; i<n; i++)
    map.add(8, min[i], max[i]);

// Create a couple of genomes for keeping track in our user data.
  data.picture_genome = new GA2DBinaryStringGenome(data.width, data.height,
						   PictureObjective, 
						   (void *)&data);
  data.numbers_genome = new GABin2DecGenome(map,
					    NumbersObjective, 
					    (void *)&data);

// Now create the GA and run it.

  GA2DBinaryStringGenome picture_genome(*(data.picture_genome));
  GABin2DecGenome numbers_genome(*(data.numbers_genome));
  GAIncrementalGA ga(picture_genome);
  ga.parameters(params);
  ga.evolve();

// Now that we have evolved the best solution, put the best into our genomes
// then print them out.

  picture_genome = ga.statistics().bestIndividual();
  std::cout << "the ga generated:\n";
  for(j=0; j<data.height; j++){
    for(i=0; i<data.width; i++)
      std::cout << (picture_genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

  numbers_genome = *data.numbers_genome;
  for(unsigned int jj=0; jj<map.nPhenotypes(); jj++){
    std::cout.width(10);
    std::cout << numbers_genome.phenotype(jj) << " ";
  }
  std::cout << "\n\n"; std::cout.flush();

  std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";


// free up all of the space we were using.
  
  for(i=0; i<data.width; i++)
    delete data.picture_target[i];
  delete [] data.picture_target;
  delete [] data.numbers_target;

  delete data.picture_genome;
  //  delete data.numbers_genome;
  
  return ga.statistics();
}

// This is the primary objective function.  If it gets a genome whose score
// is sufficiently high, then it runs another GA on the number sequence and
// returns a composite score.
float
PictureObjective(GAGenome & c)
{
  auto & genome = (GA2DBinaryStringGenome &)c;
  auto * data = (UserData *)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; j<genome.height(); j++)
      value += (float)(genome.gene(i,j) == data->picture_target[i][j]);

  float correct = value / ((float)genome.width() * (float)genome.height());

  // if we get at least 95% of the pixels right, then run the secondary ga.

  if(correct > 0.95) {
    auto& num_genome = (GABin2DecGenome&)(*data->numbers_genome);
    GAIncrementalGA ga(num_genome);
    ga.populationSize(550);
    ga.nGenerations(50);
    ga.pMutation(0.01);
    ga.pCrossover(0.9);
    ga.evolve();
    *data->numbers_genome = ga.statistics().bestIndividual();

    correct += data->numbers_genome->score();
  }

  return correct;
}


// This is the objective function for matching the sequence of numbers.
float
NumbersObjective(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;
  auto * data = (UserData *)c.userData();

  float value=genome.nPhenotypes();
  for(int i=0; i<genome.nPhenotypes(); i++)
    value += 1.0 / (1.0 + fabs(genome.phenotype(i) - data->numbers_target[i]));

  value /= genome.nPhenotypes();

  return value;
}
