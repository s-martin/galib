#ifndef EX18_HPP
#define EX18_HPP

#include <ga/ga.h>
#include <ga/std_stream.h>
#include <ga/GA2DBinStrGenome.h>

float objective(GAGenome & c)
{
  auto & genome = (GA2DBinaryStringGenome &)c;
  auto **pattern = (short **)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; i<genome.height(); j++)
      value += (float)(genome.gene(i,j) == pattern[i][j]);

  cntr++;

  return(value);
}

GAStatistics example18(GAParameterList &params, unsigned int seed, short **target, int whichGA)
{
  GASimpleGA ga;
  GA2DBinaryStringGenome genome(width, height, objective);
  genome.userData((void *)target);

  switch(whichGA){
    case SIMPLE:
      ga = GASimpleGA(genome);
      break;
    case STEADY_STATE:
      ga = GASteadyStateGA(genome);
      break;
    case INCREMENTAL:
      ga = GAIncrementalGA(genome);
      break;
  }

  ga.parameters(params);
  ga.initialize(seed);
  while(!ga.done()){
    ga.step();
  }

  return ga.statistics();
}

#endif
