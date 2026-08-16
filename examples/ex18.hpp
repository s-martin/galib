#ifndef EX18_HPP
#define EX18_HPP

#include <ga.h>

#include <memory>

inline int cntr = 0;

enum Example18GAType
{
	SIMPLE = 0,
	STEADY_STATE = 1,
	INCREMENTAL = 2
};

inline float objective(GAGenome &c)
{
	auto &genome = (GA2DBinaryStringGenome &)c;
	auto **pattern = (short **)c.userData();

	float value = 0.0;
	for (int i = 0; i < genome.width(); i++)
		for (int j = 0; j < genome.height(); j++)
			value += (float)(genome.gene(i, j) == pattern[i][j]);

	cntr++;
	return value;
}

inline GAStatistics example18(GAParameterList &params, unsigned int seed, short **target, int whichGA)
{
	const int height = 5;
	const int width = 10;

	GA2DBinaryStringGenome genome(width, height, objective);
	genome.userData((void *)target);

	std::unique_ptr<GAGeneticAlgorithm> ga;
	switch (whichGA)
	{
		case STEADY_STATE:
			ga = std::make_unique<GASteadyStateGA>(genome);
			break;
		case INCREMENTAL:
			ga = std::make_unique<GAIncrementalGA>(genome);
			break;
		case SIMPLE:
		default:
			ga = std::make_unique<GASimpleGA>(genome);
			break;
	}

	ga->parameters(params);
	ga->initialize(seed);
	while (!ga->done())
		ga->step();

	return ga->statistics();
}

#endif
