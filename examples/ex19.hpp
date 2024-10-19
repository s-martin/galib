#pragma once

#include <GABin2DecGenome.h>
#include <GASteadyStateGA.h>
#include <GASigmaTruncationScaling.h>
#include <GAParameterList.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>

double Gauss(double mean, double variance);

float DeJong1(GAGenome &);
float DeJong2(GAGenome &);
float DeJong3(GAGenome &);
float DeJong4(GAGenome &);
float DeJong5(GAGenome &);

GAGenome::Evaluator objective[5] = {DeJong1,DeJong2,DeJong3,DeJong4,DeJong5};

float objective(GAGenome &c)
{
  return objective[0](c);
}

GAStatistics example19(unsigned int seed, int whichFunction)
{
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

  GABin2DecPhenotype map;
  switch(whichFunction){
  case 0:
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    break;

  case 1:
    map.add(16, -2.048, 2.048);
    map.add(16, -2.048, 2.048);
    break;

  case 2:
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    break;

  case 3:
    {
      for(int j=0; j<30; j++)
	map.add(16, -1.28, 1.28);
    }
    break;

  case 4:
    map.add(16, -65.536, 65.536);
    map.add(16, -65.536, 65.536);
    break;

  default:
    map.add(16, 0, 0);
    break;
  }

  GABin2DecGenome genome(map, objective[whichFunction]);
  GASteadyStateGA ga(genome);
  ga.parameters(params);
  GASigmaTruncationScaling scaling;
  ga.scaling(scaling);

  ga.evolve(seed);

  return ga.statistics();
}

float DeJong1(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;
  float value=0;
  value += genome.phenotype(0) * genome.phenotype(0);
  value += genome.phenotype(1) * genome.phenotype(1);
  value += genome.phenotype(2) * genome.phenotype(2);
  return(value);
}

float DeJong2(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;
  float value=100.0;
  value *= genome.phenotype(0) * genome.phenotype(0) - genome.phenotype(1);
  value *= genome.phenotype(0) * genome.phenotype(0) - genome.phenotype(1);
  value += (1 - genome.phenotype(0))*(1 - genome.phenotype(0));
  return(value);
}

float DeJong3(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;
  float value=25.0;
  value -= floor(genome.phenotype(0));
  value -= floor(genome.phenotype(1));
  value -= floor(genome.phenotype(2));
  value -= floor(genome.phenotype(3));
  value -= floor(genome.phenotype(4));
  return(value);
}

float DeJong4(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;
  float value = 0;
  for(int i=0; i<30; i++){
    float v = genome.phenotype(i);
    v *= v;			// xi^2
    v *= v;			// xi^4
    v *= i;
    v += Gauss(0,1);
    value += v;
  }
  return(value);
}

float DeJong5(GAGenome & c)
{
  auto & genome = (GABin2DecGenome &)c;

  float prod,total=0.002;

  for(int j=0; j<25; j+=1) {
    float lowtot=1.0 + (double)j;
    for(int i=0; i<2; i+=1) {
      prod=1.0;
      for(int power=0; power<6; power+=1)
        prod*=genome.phenotype(i)-a[i][j];
      lowtot+=prod;
    }
    total+=1.0/lowtot;
  }

  return(500.0 - (1.0/total));
}

double Gauss(double mean, double variance){
  for(;;) {
    double u1 = GARandomDouble();
    double u2 = GARandomDouble();
    double v1 = 2 * u1 - 1;
    double v2 = 2 * u2 - 1;
    double w = (v1 * v1) + (v2 * v2);

    if (w <= 1) {
      double y = sqrt( (-2 * log(w)) / w);
      double x1 = v1 * y;
      return(x1 * sqrt(variance) + mean);
    }
  }
}
