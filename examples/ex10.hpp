#pragma once

#include <ga.h>
#include <cmath>

#define USE_RAW_SINE
#define NBITS 8

#ifdef USE_RAW_SINE
#define FUNCTION Function1
#define MIN_VALUE 0
#define MAX_VALUE 5
#else
#define FUNCTION Function2
#define MIN_VALUE -100
#define MAX_VALUE 100
#endif

float Function1(float);
float Function2(float);
float Objective(GAGenome &);
float BitDistance(const GAGenome & a, const GAGenome & b);
float PhenotypeDistance(const GAGenome & a, const GAGenome & b);

float objective(GAGenome & g)
{
    return Objective(g);
}

GAStatistics example10(unsigned int seed, int argc, char **argv)
{
    GABin2DecPhenotype map;
    map.add(NBITS, MIN_VALUE, MAX_VALUE);

    GABin2DecGenome genome(map, objective);
    genome.crossover(GABin2DecGenome::UniformCrossover);
    genome.crossoverProbability(0.6);
    genome.mutationProbability(0.01);
    genome.comparator(BitDistance);
    genome.comparator(PhenotypeDistance);

    GASimpleGA ga(genome);
    ga.populationSize(30);
    ga.nGenerations(100);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);
    ga.evolve(seed);

    return ga.statistics();
}

float Function1(float v)
{
    return 1 + sin(v * 2 * M_PI);
}

float Function2(float v)
{
    float y;
    y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
    if (v < -100 || v > 100)
        y = 0;
    if (y < 0)
        y = 0;
    return y + 0.00001;
}

float Objective(GAGenome & c)
{
    auto & genome = (GABin2DecGenome &)c;
    return FUNCTION(genome.phenotype(0));
}

float BitDistance(const GAGenome & c1, const GAGenome & c2)
{
    auto & a = (GABin2DecGenome &)c1;
    auto & b = (GABin2DecGenome &)c2;

    float x = 0;
    for (int i = a.length() - 1; i >= 0; i--)
        x += (a[i] != b[i] ? 1 : 0);

    return x / a.length();
}

float PhenotypeDistance(const GAGenome & c1, const GAGenome & c2)
{
    auto & a = (GABin2DecGenome &)c1;
    auto & b = (GABin2DecGenome &)c2;

    return fabs(a.phenotype(0) - b.phenotype(0)) / (MAX_VALUE - MIN_VALUE);
}
