#pragma once

#include <GARealGenome.h>
#include <GASStateGA.h>
#include <iostream>
#include <fstream>

#define MIN_VALUE (-2)
#define MAX_VALUE 2
#define INC 0.005
#define THRESHOLD 0.5

float objective(GAGenome &g)
{
    auto &genome = (GARealGenome &)g;
    return 1 + sin(genome.gene(0) * 2 * M_PI);
}

float comparator(const GAGenome &g1, const GAGenome &g2)
{
    auto &a = (GARealGenome &)g1;
    auto &b = (GARealGenome &)g2;
    return exp(-(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 100.0);
}

GAStatistics example24(unsigned int seed, int argc, char **argv)
{
    GARealAlleleSet alleles(MIN_VALUE, MAX_VALUE);
    GARealGenome genome(1, alleles, objective);
    GASharing scale(comparator);
    FinickySelector select;

    RestrictedMatingGA ga(genome);
    ga.selector(select); // use our selector instead of default
    ga.scaling(scale);   // set the scaling method to our sharing
    ga.populationSize(50); // how many individuals in the population
    ga.nGenerations(200);  // number of generations to evolve
    ga.pMutation(0.001);   // likelihood of mutating new offspring
    ga.pCrossover(0.9);    // likelihood of crossing over parents
    ga.scoreFilename("bog.dat"); // name of file for scores
    ga.scoreFrequency(1);        // keep the scores of every generation
    ga.flushFrequency(50);       // specify how often to write the score to disk
    ga.selectScores(GAStatistics::AllScores);
    ga.parameters("settings.txt"); // read parameters from settings file
    ga.parameters(argc, argv, true); // parse commands, complain if bogus args
    ga.evolve(seed);

    std::ofstream outfile;

    // dump the final population to file
    std::cout << "printing population to file 'population.dat'..." << std::endl;
    outfile.open("population.dat", (std::ios::out | std::ios::trunc));
    for (int i = 0; i < ga.population().size(); i++)
    {
        genome = ga.population().individual(i);
        outfile << genome.gene(0) << "\t" << genome.score() << "\n";
    }
    outfile.close();

    // dump the function to file so you can plot the population on it
    std::cout << "printing function to file 'sinusoid.dat'..." << std::endl;
    outfile.open("sinusoid.dat", (std::ios::out | std::ios::trunc));
    for (float x = MIN_VALUE; x <= MAX_VALUE; x += INC)
    {
        outfile << genome.gene(0, x) << "\t" << genome.score() << "\n";
    }
    outfile.close();

    return ga.statistics();
}
