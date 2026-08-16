#ifndef EX18_HPP
#define EX18_HPP

#include <ga.h>

extern int cntr;

enum Example18GAType
{
	SIMPLE = 0,
	STEADY_STATE = 1,
	INCREMENTAL = 2
};

float objective(GAGenome &c);
GAStatistics example18(
	GAParameterList &params,
	unsigned int seed,
	short **target,
	int whichGA,
	int width,
	int height
);

#endif
