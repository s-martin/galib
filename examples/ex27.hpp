#pragma once

#include <cmath>
#include <GASStateGA.h>
#include <GAList.hpp>
#include <GA1DArrayGenome.hpp>
#include <iostream>

float Function1(float, float);
float Function2(float, float);
float Function3(float, float);
float Function4(float, float);
float ai[25], bi[25];

static int which = 0;
static float minx[] = {-6, -60, -500, -10 };
static float maxx[] = { 6,  60,  500, 10 };
static float miny[] = {-6, -60, -500, -10 };
static float maxy[] = { 6,  60,  500, 10 };

float objective(GAGenome& g) {
  auto& genome = (GA1DArrayGenome<float>&)g;
  return (obj[which])(genome.gene(0), genome.gene(1));
}

void initializer(GAGenome& g) {
  auto& genome = (GA1DArrayGenome<float>&)g;
  genome.gene(0, GARandomFloat(minx[which], maxx[which]));
  genome.gene(1, GARandomFloat(miny[which], maxy[which]));
}

int mutator(GAGenome& g, float pmut) {
  auto& genome = (GA1DArrayGenome<float>&)g;
  int nmut = 0;

  if(GAFlipCoin(pmut)){
    genome.gene(0, genome.gene(0) + 
		10*GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    genome.gene(1, genome.gene(1) + 
		10*GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    nmut++;
  }

  if(genome.gene(0) < minx[which]) genome.gene(0, minx[which]);
  if(genome.gene(0) > maxx[which]) genome.gene(0, maxx[which]);
  if(genome.gene(1) < minx[which]) genome.gene(1, minx[which]);
  if(genome.gene(1) > maxx[which]) genome.gene(1, maxx[which]);

  return nmut;
}

int crossover(const GAGenome& g1,const GAGenome& g2,GAGenome* c1,GAGenome* c2)
{
  auto& mom = (GA1DArrayGenome<float>&)g1;
  auto& dad = (GA1DArrayGenome<float>&)g2;

  int n = 0;
  float distance = 0.0, midpoint = 0.0;

  if(c1) 
  {
    auto& sis = (GA1DArrayGenome<float>&)*c1;
    distance = midpoint = 0.0;

    midpoint = (mom.gene(0) + dad.gene(0)) / 2;
    distance = fabs(mom.gene(0) - dad.gene(0));
    sis.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    midpoint = (mom.gene(1) + dad.gene(1)) / 2;
    distance = fabs(mom.gene(1) - dad.gene(1));
    sis.gene(1, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    if(sis.gene(0) < minx[which]) sis.gene(0, minx[which]);
    if(sis.gene(0) > maxx[which]) sis.gene(0, maxx[which]);
    if(sis.gene(1) < minx[which]) sis.gene(1, minx[which]);
    if(sis.gene(1) > maxx[which]) sis.gene(1, maxx[which]);

    n += 1;
  }
  if(c2) {
    auto& bro = (GA1DArrayGenome<float>&)*c2;
    distance = midpoint = 0.0;

    midpoint = (mom.gene(0) + dad.gene(0)) / 2;
    distance = fabs(mom.gene(0) - dad.gene(0));
    bro.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    midpoint = (mom.gene(1) + dad.gene(1)) / 2;
    distance = fabs(mom.gene(1) - dad.gene(1));
    bro.gene(1, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    if(bro.gene(0) < minx[which]) bro.gene(0, minx[which]);
    if(bro.gene(0) > maxx[which]) bro.gene(0, maxx[which]);
    if(bro.gene(1) < minx[which]) bro.gene(1, minx[which]);
    if(bro.gene(1) > maxx[which]) bro.gene(1, maxx[which]);

    n += 1;
  }

  return n;
}

float comparator(const GAGenome& g1, const GAGenome& g2) {
  auto& a = (GA1DArrayGenome<float>&)g1;
  auto& b = (GA1DArrayGenome<float>&)g2;

  float valx=(a.gene(0)-b.gene(0)) * (a.gene(0)-b.gene(0));
  float valy=(a.gene(1)-b.gene(1)) * (a.gene(1)-b.gene(1));
  return sqrt(valx+valy);
}

GAStatistics example27(unsigned int seed, int argc, char **argv)
{
    for (int i = 0; i < 25; i++) {
        ai[i] = 16 * ((i % 5) - 2);
        bi[i] = 16 * ((i / 5) - 2);
    }

    GA1DArrayGenome<float> genome(2, objective);
    genome.initializer(::initializer);
    genome.mutator(::mutator);
    genome.comparator(::comparator);
    genome.crossover(::crossover);

    DCrowdingGA ga(genome);
    ga.maximize();
    ga.populationSize(100);
    ga.nGenerations(100);
    ga.pMutation(0.05);
    ga.pCrossover(1.0);
    ga.selectScores(GAStatistics::AllScores);
    ga.parameters(argc, argv, true);

    ga.evolve(seed);
    std::cout << "best individual is " << ga.statistics().bestIndividual() << "\n\n";
    std::cout << ga.statistics() << "\n";

    return ga.statistics();
}
