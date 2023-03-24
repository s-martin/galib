// $Header$
/* ----------------------------------------------------------------------------
  gabase.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

  Source file for the base genetic algorithm object.
---------------------------------------------------------------------------- */
#include <GABaseGA.h>
#include <cstdio>
#include <cstring>
#include <garandom.h>
#include <gaversion.h> // gets the RCS string in for ident purposes

#include <boost/algorithm/string.hpp>

// return the configuration string that identifies this build of the library.
static const char *rcsid = GALIB_LIBRARY_IDENTIFIER;
const char *GAConfig() { return rcsid; }

// Here are a few termination functions that you can use.  Terminators return
// true if the algorithm should finish, false otherwise.
bool GAGeneticAlgorithm::TerminateUponGeneration(GAGeneticAlgorithm &ga)
{
	return (ga.generation() < ga.nGenerations() ? false : true);
}

// If we are maximizing, then terminate when the convergence has exceeded the
// specified convergence.  If we are minimizing, then terminate when the
// convergence has dropped below the specified convergence.
bool GAGeneticAlgorithm::TerminateUponConvergence(GAGeneticAlgorithm &ga)
{
	bool val = false;
	if (ga.minimaxi() == GAGeneticAlgorithm::MINIMIZE)
	{
		if (ga.convergence() == 0 || ga.convergence() > ga.pConvergence())
		{
			val = false;
		}
		else
		{
			val = true;
		}
	}
	else
	{
		if (ga.convergence() < ga.pConvergence())
		{
			val = false;
		}
		else
		{
			val = true;
		}
	}
	return val;
}

// Use the ratio between the minimum and maximum to determine whether the
// population has converged.  This method will not work if the values cross
// zero!
// Note that this is significantly different than the definition (and the
// bug-laden implementation) that was in version of GAlib prior to 2.4.5.
//
// For historical purposes, here is the old definition of this method:
//
// Use the ratio of the population mean divided by the population maximum to
// determine whether the population has converged.  If we are maximizing, then
// check to see if the ratio exceeds the convergence.  If we are minimizing,
// then check to see if the ratio has dropped below the convergence.
bool GAGeneticAlgorithm::TerminateUponPopConvergence(GAGeneticAlgorithm &ga)
{
	bool val = false;

	if (ga.statistics().current(GAStatistics::Maximum) == 0)
	{
		return val;
	}

	float ratio = ga.statistics().current(GAStatistics::Minimum) /
				  ga.statistics().current(GAStatistics::Maximum);

	if (ga.minimaxi() == GAGeneticAlgorithm::MINIMIZE)
	{
		if (ratio <= ga.pConvergence())
		{
			val = true;
		}
		else
		{
			val = false;
		}
	}
	else
	{
		if (ratio >= ga.pConvergence())
		{
			val = true;
		}
		else
		{
			val = false;
		}
	}

	return val;
}


// When we create a GA, we stuff the parameters with the basics that will be
// needed by most genetic algorithms - num generations, p convergence, etc.
GAGeneticAlgorithm::GAGeneticAlgorithm(const GAGenome &g, const std::shared_ptr<GAParameterList>& _params) :
	params(_params)
{
	pop = new GAPopulation(g, params->numPopulations);
	pop->geneticAlgorithm(*this);

	ud = nullptr;
	cf = GAGeneticAlgorithm::DEFAULT_TERMINATOR;

	d_seed = params->seed;

	minmax = params->miniMaxi;
	ngen = params->numGenerations;
	nconv = params->generationsToConvergence;
	stats.nConvergence(nconv);
	pconv = params->convergencePercentage;
	pcross = params->probCrossover;
	pmut = params->probMutation;
	int psize = pop->size();

	stats.scoreFrequency(params->scoreFrequency);
	stats.flushFrequency(params->flushFrequency);
	stats.recordDiversity(params->recordDiversity);
	stats.scoreFilename(params->scoreFilename);
	stats.selectScores(params->selectScores);
	stats.nBestGenomes(g, params->numberOfBest);

	scross = g.sexual();
	across = g.asexual();
}

GAGeneticAlgorithm::GAGeneticAlgorithm(const GAPopulation &p, const std::shared_ptr<GAParameterList>& _params) :
	params(_params)
{
	pop = new GAPopulation(p);
	pop->geneticAlgorithm(*this);

	ud = nullptr;
	cf = GAGeneticAlgorithm::DEFAULT_TERMINATOR;

	d_seed = params->seed;

	minmax = params->miniMaxi;
	ngen = params->numGenerations;
	nconv = params->generationsToConvergence;
	stats.nConvergence(nconv);
	pconv = params->convergencePercentage;
	pcross = params->probCrossover;
	pmut = params->probMutation;
	int psize = pop->size();

	stats.scoreFrequency(params->scoreFrequency);
	stats.flushFrequency(params->flushFrequency);
	stats.recordDiversity(params->recordDiversity);
	stats.scoreFilename(params->scoreFilename);
	stats.selectScores(params->selectScores);
	stats.nBestGenomes(p.individual(0), params->numberOfBest);

	scross = p.individual(0).sexual();
	across = p.individual(0).asexual();
}

GAGeneticAlgorithm::GAGeneticAlgorithm(const GAGeneticAlgorithm &ga)
	: stats(ga.stats), params(ga.params)
{
	pop = ga.pop->clone();
	pop->geneticAlgorithm(*this);

	cf = ga.cf;
	ud = ga.ud;
	ngen = ga.ngen;
	nconv = ga.nconv;
	pconv = ga.pconv;
	pcross = ga.pcross;
	pmut = ga.pmut;
	minmax = ga.minmax;
	scross = ga.scross;
	across = ga.across;
	d_seed = ga.d_seed;
}

GAGeneticAlgorithm::~GAGeneticAlgorithm() 
{ 
	delete pop; 
}

void GAGeneticAlgorithm::copy(const GAGeneticAlgorithm &ga)
{
	if (pop != nullptr)
	{
		pop->copy(*(ga.pop));
	}
	else
	{
		pop = ga.pop->clone();
	}
	pop->geneticAlgorithm(*this);

	stats = ga.stats;
	params = ga.params;

	cf = ga.cf;
	ud = ga.ud;
	ngen = ga.ngen;
	nconv = ga.nconv;
	pconv = ga.pconv;
	pcross = ga.pcross;
	pmut = ga.pmut;
	minmax = ga.minmax;
	scross = ga.scross;
	across = ga.across;
	d_seed = ga.d_seed;
}


void GAGeneticAlgorithm::objectiveFunction(GAGenome::Evaluator f)
{
	for (int i = 0; i < pop->size(); i++)
	{
		pop->individual(i).evaluator(f);
	}
}

void GAGeneticAlgorithm::objectiveData(const GAEvalData &v)
{
	for (int i = 0; i < pop->size(); i++)
	{
		pop->individual(i).evalData(v);
	}
}

const GAPopulation &GAGeneticAlgorithm::population(const GAPopulation &p)
{
	if (p.size() < 1)
	{
		GAErr(GA_LOC, className(), "population", GAError::NoIndividuals);
		return *pop;
	}

	pop->copy(p);
	pop->geneticAlgorithm(*this);

	return *pop;
}

int GAGeneticAlgorithm::populationSize(unsigned int value)
{
	unsigned int ps = value;
	params->set(gaNpopulationSize, value);
	return pop->size(ps);
}

int GAGeneticAlgorithm::minimaxi(int m)
{
	if (m == MINIMIZE)
	{
		pop->order(GAPopulation::LOW_IS_BEST);
	}
	else
	{
		pop->order(GAPopulation::HIGH_IS_BEST);
	}
	params->set(gaNminimaxi, m);
	minmax = (m == MINIMIZE ? MINIMIZE : MAXIMIZE);
	return minmax;
}
