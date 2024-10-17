#pragma once

#include <ga.h>
#include <cmath>

#define MIN_VALUE (-2)
#define MAX_VALUE 2
#define INC 0.005

float objective(GAGenome &g)
{
    auto &genome = (GARealGenome &)g;
    return 1 + sin(genome.gene(0) * 2 * M_PI);
}

float comparator(const GAGenome &g1, const GAGenome &g2)
{
    auto &a = (GARealGenome &)g1;
    auto &b = (GARealGenome &)g2;
    return exp(-(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 1000.0);
}

GAStatistics example23(unsigned int seed, int argc, char **argv)
{
    GARealAlleleSet alleles(MIN_VALUE, MAX_VALUE, INC);
    GARealGenome genome(alleles, objective);
    genome.comparator(comparator);

    GASimpleGA ga(genome);
    ga.populationSize(30);
    ga.nGenerations(100);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);
    ga.evolve(seed);

    return ga.statistics();
}
