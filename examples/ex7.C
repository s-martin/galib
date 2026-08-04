#include "ex7.hpp"
#include <GA2DBinStrGenome.h>
#include <GASStateGA.h>
#include <fstream>
#include <iostream>

// objectiveEx7 moved from header to here
float objectiveEx7(GAGenome& c)
{
    auto& genome = (GA2DBinaryStringGenome&)c;
    auto* pattern = (GA2DBinaryStringGenome*)c.userData();

    float value = 0.0f;
    for (int i = 0; i < genome.width(); ++i)
        for (int j = 0; j < genome.height(); ++j)
            value += (float)(genome.gene(i, j) == pattern->gene(i, j));
    return value;
}

float objective(GAGenome& c)
{
    return objectiveEx7(c);
}

// existing example7 implementation (kept, but now uses the above definitions)
GAStatistics example7(GAParameterList params, const std::string &datafile)
{
    std::cout << "Example 7\n\n";
    std::cout << "This program reads in a data file then runs a steady-state GA \n";
    std::cout << "whose objective function tries to match the pattern of bits that\n";
    std::cout << "are in the data file.\n\n";

    std::ifstream inStream(datafile);
    if (!inStream)
    {
        std::cerr << "Cannot open " << datafile << " for input.\n";
        exit(1);
    }

    int height, width;
    inStream >> height >> width;
    GA2DBinaryStringGenome target(width, height);
    inStream >> target;
    inStream.close();

    std::cout << "input pattern:\n";
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
            std::cout << (target.gene(i, j) == 1 ? '*' : ' ') << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout.flush();

    GA2DBinaryStringGenome genome(width, height, objectiveEx7, (void*)&target);
    GASteadyStateGA ga(genome);
    ga.parameters(params);

    GATournamentSelector selector;
    ga.selector(selector);

    ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);

    ga.initialize();
    while (!ga.done())
    {
        ++ga;
    }
    ga.flushScores();

    genome = ga.statistics().bestIndividual();
    std::cout << "the ga generated:\n";
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
            std::cout << (genome.gene(i, j) == 1 ? '*' : ' ') << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout.flush();

    std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

    return ga.statistics();
}
