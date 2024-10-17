#pragma once

#include <GARealGenome.h>
#include <GASimpleGA.h>
#include <iostream>

float objective(GAGenome &g)
{
    auto &genome = (GARealGenome &)g;
    float score = 0.0;
    for (int i = 0; i < genome.length(); i++)
    {
        score += genome.gene(i);
    }
    return score;
}

GAStatistics example21(unsigned int seed, int argc, char **argv)
{
    GARealAlleleSetArray alleles;
    alleles.add(0, 1);
    alleles.add(0, 1);
    alleles.add(0, 1);
    alleles.add(0, 1);
    alleles.add(0, 1);

    GARealGenome genome(alleles, objective);

    GASimpleGA ga(genome);
    ga.populationSize(30);
    ga.nGenerations(100);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);
    ga.evolve(seed);

    return ga.statistics();
}
