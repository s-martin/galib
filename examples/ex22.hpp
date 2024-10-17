#pragma once

#include <ga.h>
#include <cmath>
#include <iostream>

float objective(GAGenome &g)
{
    auto &genome = (GA1DArrayGenome<float> &)g;
    float v = genome.gene(0);
    float y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
    if (v < -100 || v > 100)
        y = 0;
    if (y < 0)
        y = 0;
    return y + 0.00001;
}

GAStatistics example22(unsigned int seed, int argc, char **argv)
{
    GA1DArrayGenome<float> genome(1, objective);
    genome.initializer(::Initializer);
    genome.mutator(::Mutator);
    genome.crossover(::Crossover);

    SharedOverlapGA ga(genome);
    ga.populationSize(30);
    ga.nGenerations(100);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);
    ga.evolve(seed);

    return ga.statistics();
}
