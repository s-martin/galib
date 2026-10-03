#ifndef EX18_HPP
#define EX18_HPP

#include <ga.h>
#include <GA2DBinStrGenome.h>

#include <memory>

enum Ex18GAType
{
  SIMPLE = 0,
  STEADY_STATE = 1,
  INCREMENTAL = 2
};

// counts how often the objective function was called
int cntr = 0;

float objectiveEx18(GAGenome & c)
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

GAStatistics example18(GAParameterList &params, unsigned int seed, short **target, int width, int height, int whichGA)
{
  GA2DBinaryStringGenome genome(width, height, objectiveEx18);
  genome.userData((void *)target);

  std::unique_ptr<GAGeneticAlgorithm> ga;
  switch(whichGA){
    case STEADY_STATE:
      ga = std::make_unique<GASteadyStateGA>(genome);
      break;
    case INCREMENTAL:
      ga = std::make_unique<GAIncrementalGA>(genome);
      break;
    case SIMPLE:
    default:
      ga = std::make_unique<GASimpleGA>(genome);
      break;
  }

  ga->parameters(params);
  ga->initialize(seed);
  while(!ga->done()){
    ga->step();
  }

  return ga->statistics();
}

#endif
