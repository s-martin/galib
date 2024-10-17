#pragma once

#include <GA1DBinStrGenome.h>
#include <GADemeGA.h>
#include <iostream>

float objective(GAGenome &g)
{
    auto &genome = (GA1DBinaryStringGenome &)g;
    float score = 0.0;
    for (int i = 0; i < genome.length(); i++)
        score += genome.gene(i);
    score /= genome.length();
    return score;
}

GAStatistics example25(unsigned int seed, int argc, char **argv)
{
    std::cout << "Example 25\n\n";
    std::cout << "This example uses a genetic algorithm with multiple populations.\n";
    std::cout << std::endl;

    GADemeGA ga;
    GA1DBinaryStringGenome genome(100, objective);
    ga.populationSize(50);
    ga.nGenerations(200);
    ga.pMutation(0.01);
    ga.pCrossover(0.6);
    ga.nPopulations(5);
    ga.migrationFrequency(10);
    ga.migrationSize(5);
    ga.evolve(seed);

    return ga.statistics();
}
