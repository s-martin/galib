#pragma once

#include <ga.h>
#include <iostream>

#define NBLOCKS 16
const int BLOCKSIZE = 8;
const int GAPSIZE = 7;
const int MSTAR = 4;
const float USTAR = 1.0;
const float RR_U = 0.3;
const float RR_V = 0.02;

int nbits = (BLOCKSIZE + GAPSIZE) * NBLOCKS;
int blockarray[NBLOCKS];
int highestLevel = 0;

float RoyalRoad(GAGenome &c)
{
    auto &genome = (GA1DBinaryStringGenome &)c;

    float score = 0.0;
    int total, i, j, index, n;

    n = 0;
    for (i = 0; i < NBLOCKS; i++)
    {
        total = 0;
        for (j = i * (BLOCKSIZE + GAPSIZE); j < i * (BLOCKSIZE + GAPSIZE) + BLOCKSIZE; j++)
            if (genome.gene(j) == 1)
                total++;
        if (total > MSTAR && total < BLOCKSIZE)
            score -= (total - MSTAR) * RR_V;
        else if (total <= MSTAR)
            score += total * RR_V;
        if (total == BLOCKSIZE)
        {
            blockarray[i] = 1;
            n++;
        }
        else
        {
            blockarray[i] = 0;
        }
    }

    if (n > 0)
        score += USTAR + (n - 1) * RR_U;

    n = NBLOCKS;
    int proceed = 1;
    int level = 0;
    while ((n > 1) && proceed)
    {
        proceed = 0;
        total = 0;
        for (i = 0, index = 0; i < (n / 2) * 2; i += 2, index++)
        {
            if (blockarray[i] == 1 && blockarray[i + 1] == 1)
            {
                total++;
                proceed = 1;
                blockarray[index] = 1;
            }
            else
            {
                blockarray[index] = 0;
            }
        }
        if (total > 0)
        {
            score += USTAR + (total - 1) * RR_U;
            level++;
        }
        n /= 2;
    }

    if (highestLevel < level)
        highestLevel = level;

    return (score);
}

float objective(GAGenome &c)
{
    return RoyalRoad(c);
}

GAStatistics example20(unsigned int seed, int argc, char **argv)
{
    std::cout << "Example 20\n\n";
    std::cout << "Running Holland's Royal Road test problem with a genome that is\n";
    std::cout << nbits << " bits long (" << NBLOCKS << " blocks).  The parameters ";
    std::cout << "are as follows: \n\n";
    std::cout << "\tblock size: " << BLOCKSIZE << "\n";
    std::cout << "\t  gap size: " << GAPSIZE << "\n";
    std::cout << "\t        m*: " << MSTAR << "\n";
    std::cout << "\t        u*: " << USTAR << "\n";
    std::cout << "\t         u: " << RR_U << "\n";
    std::cout << "\t         v: " << RR_V << "\n";
    std::cout << "\n\n";
    std::cout.flush();

    for (int ii = 1; ii < argc; ii++)
    {
        if (strcmp(argv[ii++], "seed") == 0)
        {
            seed = atoi(argv[ii]);
        }
    }

    GA1DBinaryStringGenome genome(nbits, RoyalRoad);
    GASteadyStateGA ga(genome);
    ga.populationSize(512);
    ga.pCrossover(0.9);
    ga.pMutation(0.001);
    ga.nGenerations(10000);
    ga.scoreFilename("bog.dat");
    ga.flushFrequency(100);
    ga.scoreFrequency(20);
    ga.parameters(argc, argv);
    GASigmaTruncationScaling trunc;
    ga.scaling(trunc);
    ga.evolve(seed);

    std::cout << "the ga generated:\n"
              << ga.statistics().bestIndividual() << "\n";
    std::cout << "the highest level achieved was " << highestLevel << "\n";
    std::cout << "\nthe statistics for the run are:\n"
              << ga.statistics();
    std::cout << "\nthe parameters for the run are:\n"
              << ga.parameters();
    std::cout.flush();

    return ga.statistics();
}
