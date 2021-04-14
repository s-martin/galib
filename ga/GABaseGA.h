// $Header$
/* ----------------------------------------------------------------------------
  gabase.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

  Header for the base genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_gabase_h_
#define _ga_gabase_h_

#include <GAGenome.h>
#include <GAParameter.h>
#include <GAPopulation.h>
#include <GAStatistics.h>
#include <gaconfig.h>
#include <gaid.h>

constexpr int gaDefScoreFrequency2 = 100;

/* ----------------------------------------------------------------------------
   The base GA class is virtual - it defines the core data elements and parts
of the interface that are common to all genetic algorithms (as defined in
GAlib, that is).

initialize
  Undefined for the base class.  The initialization routine typically calls
the population initializer (which typically calls the genome initializers).
It should also reset the statistics.

step
  Evolve by one generation.  'generation' can be defined different ways for
different genetic algorithms, but in the traditional formulation a generation
mean creation of a new population (or portion thereof).

done
  Calls the completion measure routine to tell whether or not the GA is done.

evolve
  This method is provided as a convenience so that you don't have to increment
the GA generation-by-generation by hand.  If you do decide to do it by hand,
be sure that you initialize before you start evolving!
---------------------------------------------------------------------------- */
class GAGeneticAlgorithm : public GAID
{
  public:
	GADefineIdentity("GAIncrementalGA", GAID::BaseGA);

	using Terminator = bool (*)(GAGeneticAlgorithm &);

	enum
	{
		MINIMIZE = -1,
		MAXIMIZE = 1
	};

	static bool TerminateUponGeneration(GAGeneticAlgorithm &);
	static bool TerminateUponConvergence(GAGeneticAlgorithm &);
	static bool TerminateUponPopConvergence(GAGeneticAlgorithm &);

  public:
	GAGeneticAlgorithm(const GAGenome &, const std::shared_ptr<GAParameterList> &_params);
	GAGeneticAlgorithm(const GAPopulation &, const std::shared_ptr<GAParameterList> &_params);
	GAGeneticAlgorithm(const GAGeneticAlgorithm &);
	~GAGeneticAlgorithm() override;
	virtual void copy(const GAGeneticAlgorithm &);

	bool done() { return (*cf)(*this); }
	virtual void initialize(unsigned int seed = 0) = 0;
	virtual void step() = 0;
	virtual void evolve(unsigned int seed = 0)
	{
		initialize(seed);
		while (!done())
		{
			step();
		}
		if (stats.flushFrequency() > 0)
		{
			stats.flushScores();
		}
	}
	virtual int write(const char *) const { return 0; }
	virtual int write(std::ostream &) const { return 0; }
	virtual int read(const char *) { return 0; }
	virtual int read(std::istream &) { return 0; }

	void *userData() const { return ud; }
	void *userData(void *d) { return ud = d; }
	Terminator terminator() const { return cf; }
	Terminator terminator(Terminator f) { return cf = f; }

	virtual int minimaxi() const { return minmax; }
	virtual int minimaxi(int m);
	int minimize() { return minimaxi(MINIMIZE); }
	int maximize() { return minimaxi(MAXIMIZE); }
	int nGenerations() const { return ngen; }
	int nGenerations(unsigned int n)
	{
		params->set(gaNnGenerations, n);
		return ngen = n;
	}
	int nConvergence() const { return nconv; }
	int nConvergence(unsigned int n)
	{
		params->set(gaNnConvergence, n);
		return nconv = stats.nConvergence(n);
	}
	float pConvergence() const { return pconv; }
	float pConvergence(float p)
	{
		params->set(gaNpConvergence, p);
		return pconv = p;
	}
	float pCrossover() const { return pcross; }
	float pCrossover(float p)
	{
		params->set(gaNpCrossover, p);
		return pcross = p;
	}
	float pMutation() const { return pmut; }
	float pMutation(float p)
	{
		params->set(gaNpMutation, p);
		return pmut = p;
	}

	GAGenome::SexualCrossover crossover(GAGenome::SexualCrossover f)
	{
		return scross = f;
	}
	GAGenome::SexualCrossover sexual() const { return scross; }
	GAGenome::AsexualCrossover crossover(GAGenome::AsexualCrossover f)
	{
		return across = f;
	}
	GAGenome::AsexualCrossover asexual() const { return across; }

	const GAStatistics &statistics() const { return stats; }
	float convergence() const { return stats.convergence(); }
	int generation() const { return stats.generation(); }
	void flushScores()
	{
		if (stats.flushFrequency() > 0)
		{
			stats.flushScores();
		}
	}

	int scoreFrequency() const { return stats.scoreFrequency(); }
	int scoreFrequency(unsigned int x)
	{
		params->set(gaNscoreFrequency, x);
		return stats.scoreFrequency(x);
	}
	int flushFrequency() const { return stats.flushFrequency(); }
	int flushFrequency(unsigned int x)
	{
		params->set(gaNflushFrequency, x);
		return stats.flushFrequency(x);
	}
	std::string scoreFilename() const { return stats.scoreFilename(); }
	std::string scoreFilename(const std::string &fn)
	{
		params->set(gaNscoreFilename, fn.c_str());
		return stats.scoreFilename(fn);
	}
	int selectScores() { return stats.selectScores(); }
	int selectScores(int w)
	{
		params->set(gaNselectScores, w);
		return stats.selectScores(w);
	}
	bool recordDiversity() const { return stats.recordDiversity(); }
	bool recordDiversity(bool f)
	{
		params->set(gaNrecordDiversity, static_cast<int>(f));
		return stats.recordDiversity(f);
	}

	virtual const GAPopulation &population() const { return *pop; }
	virtual const GAPopulation &population(const GAPopulation &);
	virtual int populationSize() const { return pop->size(); }
	virtual int populationSize(unsigned int value);
	virtual int nBestGenomes() const { return stats.nBestGenomes(); }
	virtual int nBestGenomes(unsigned int n)
	{
		params->set(gaNnBestGenomes, n);
		return stats.nBestGenomes(pop->individual(0), n);
	}

	virtual GAScalingScheme &scaling() const { return pop->scaling(); }
	virtual GAScalingScheme &scaling(const GAScalingScheme &s)
	{
		return pop->scaling(s);
	}
	virtual GASelectionScheme &selector() const { return pop->selector(); }
	virtual GASelectionScheme &selector(const GASelectionScheme &s)
	{
		return pop->selector(s);
	}
	virtual void objectiveFunction(GAGenome::Evaluator f);
	virtual void objectiveData(const GAEvalData &v);

  protected:
	GAStatistics stats;
	std::shared_ptr<GAParameterList> params;
	GAPopulation *pop;
	/// function for determining done-ness
	Terminator cf; 
	/// pointer to user data structure
	void *ud; 

	int d_seed;
	unsigned int ngen;
	unsigned int nconv;
	float pconv;
	float pcross;
	float pmut;
	int minmax;
	/// sexual crossover to use
	GAGenome::SexualCrossover scross; 
	/// asexual crossover to use
	GAGenome::AsexualCrossover across; 
};

#endif
