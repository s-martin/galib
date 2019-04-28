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

// When specifying parameters for a GAlib object, you can use the fullname (the
// name used in parameters data files) or the short name (the name typically
// used on the command line).  When specifying parameters in your code you can
// use a string, or use the predefined macros below (kind of like using the
// resource/class names in Motif for you Xt jocks).
constexpr auto gaNnGenerations = "number_of_generations";
constexpr auto gaSNnGenerations = "ngen";
constexpr auto gaNpConvergence = "convergence_percentage";
constexpr auto gaSNpConvergence = "pconv";
constexpr auto gaNnConvergence = "generations_to_convergence";
constexpr auto gaSNnConvergence = "nconv";
constexpr auto gaNpCrossover = "crossover_probability";
constexpr auto gaSNpCrossover = "pcross";
constexpr auto gaNpMutation = "mutation_probability";
constexpr auto gaSNpMutation = "pmut";
constexpr auto gaNpopulationSize = "population_size";
constexpr auto gaSNpopulationSize = "popsize";
constexpr auto gaNnPopulations = "number_of_populations";
constexpr auto gaSNnPopulations = "npop";
constexpr auto gaNpReplacement = "replacement_percentage";
constexpr auto gaSNpReplacement = "prepl";
constexpr auto gaNnReplacement = "replacement_number";
constexpr auto gaSNnReplacement = "nrepl";
constexpr auto gaNnBestGenomes = "number_of_best";
constexpr auto gaSNnBestGenomes = "nbest";
constexpr auto gaNscoreFrequency = "score_frequency";
constexpr auto gaSNscoreFrequency = "sfreq";
constexpr auto gaNflushFrequency = "flush_frequency";
constexpr auto gaSNflushFrequency = "ffreq";
constexpr auto gaNscoreFilename = "score_filename";
constexpr auto gaSNscoreFilename = "sfile";
constexpr auto gaNselectScores = "select_scores";
constexpr auto gaSNselectScores = "sscores";
constexpr auto gaNelitism = "elitism";
constexpr auto gaSNelitism = "el";
constexpr auto gaNnOffspring = "number_of_offspring";
constexpr auto gaSNnOffspring = "noffspr";
constexpr auto gaNrecordDiversity = "record_diversity";
constexpr auto gaSNrecordDiversity = "recdiv";
constexpr auto gaNpMigration = "migration_percentage";
constexpr auto gaSNpMigration = "pmig";
constexpr auto gaNnMigration = "migration_number";
constexpr auto gaSNnMigration = "nmig";
constexpr auto gaNminimaxi = "minimaxi";
constexpr auto gaSNminimaxi = "mm";
constexpr auto gaNseed = "seed";
constexpr auto gaSNseed = "seed";

extern int gaDefNumGen;
extern float gaDefPConv;
extern int gaDefNConv;
extern float gaDefPMut;
extern float gaDefPCross;
extern int gaDefPopSize;
extern int gaDefNPop;
extern float gaDefPRepl;
extern int gaDefNRepl;
extern int gaDefNumOff;
extern float gaDefPMig;
extern int gaDefNMig;
extern int gaDefSelectScores;
extern int gaDefMiniMaxi;
extern bool gaDefDivFlag;
extern bool gaDefElitism;
extern int gaDefSeed;

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

	typedef bool (*Terminator)(GAGeneticAlgorithm &ga);

	enum
	{
		MINIMIZE = -1,
		MAXIMIZE = 1
	};

	static GAParameterList &registerDefaultParameters(GAParameterList &);

	static bool TerminateUponGeneration(GAGeneticAlgorithm &);
	static bool TerminateUponConvergence(GAGeneticAlgorithm &);
	static bool TerminateUponPopConvergence(GAGeneticAlgorithm &);

  public:
	explicit GAGeneticAlgorithm(const GAGenome &);
	explicit GAGeneticAlgorithm(const GAPopulation &);
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

	const GAParameterList &parameters() const { return params; }
	const GAParameterList &parameters(const GAParameterList &);
	const GAParameterList &parameters(int &, char **, bool flag = false);
	const GAParameterList &parameters(const char *filename, bool f = false);
	const GAParameterList &parameters(std::istream &, bool flag = false);
	virtual int get(const char *, void *) const;
	virtual int setptr(const std::string &name, const void *);
	int set(const std::string &s, int v) { return setptr(s, (void *)&v); }
	int set(const std::string &s, unsigned int v) { return setptr(s, (void *)&v); }
	int set(const std::string &s, char v) { return setptr(s, (void *)&v); }
	int set(const std::string &s, const char *v) { return setptr(s, (void *)v); }
	int set(const std::string &s, const void *v) { return setptr(s, v); }
	int set(const std::string &name, double v);

	virtual int minimaxi() const { return minmax; }
	virtual int minimaxi(int m);
	int minimize() { return minimaxi(MINIMIZE); }
	int maximize() { return minimaxi(MAXIMIZE); }
	int nGenerations() const { return ngen; }
	int nGenerations(unsigned int n)
	{
		params.set(gaNnGenerations, n);
		return ngen = n;
	}
	int nConvergence() const { return nconv; }
	int nConvergence(unsigned int n)
	{
		params.set(gaNnConvergence, n);
		return nconv = stats.nConvergence(n);
	}
	float pConvergence() const { return pconv; }
	float pConvergence(float p)
	{
		params.set(gaNpConvergence, p);
		return pconv = p;
	}
	float pCrossover() const { return pcross; }
	float pCrossover(float p)
	{
		params.set(gaNpCrossover, p);
		return pcross = p;
	}
	float pMutation() const { return pmut; }
	float pMutation(float p)
	{
		params.set(gaNpMutation, p);
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
		params.set(gaNscoreFrequency, x);
		return stats.scoreFrequency(x);
	}
	int flushFrequency() const { return stats.flushFrequency(); }
	int flushFrequency(unsigned int x)
	{
		params.set(gaNflushFrequency, x);
		return stats.flushFrequency(x);
	}
	const char *scoreFilename() const { return stats.scoreFilename(); }
	const char *scoreFilename(const char *fn)
	{
		params.set(gaNscoreFilename, fn);
		return stats.scoreFilename(fn);
	}
	int selectScores() { return stats.selectScores(); }
	int selectScores(int w)
	{
		params.set(gaNselectScores, w);
		return stats.selectScores(w);
	}
	bool recordDiversity() const { return stats.recordDiversity(); }
	bool recordDiversity(bool f)
	{
		params.set(gaNrecordDiversity, static_cast<int>(f));
		return stats.recordDiversity(f);
	}

	virtual const GAPopulation &population() const { return *pop; }
	virtual const GAPopulation &population(const GAPopulation &);
	virtual int populationSize() const { return pop->size(); }
	virtual int populationSize(unsigned int value);
	virtual int nBestGenomes() const { return stats.nBestGenomes(); }
	virtual int nBestGenomes(unsigned int n)
	{
		params.set(gaNnBestGenomes, n);
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
	GAParameterList params;
	GAPopulation *pop;
	Terminator cf; // function for determining done-ness
	void *ud; // pointer to user data structure

	int d_seed;
	unsigned int ngen;
	unsigned int nconv;
	float pconv;
	float pcross;
	float pmut;
	int minmax;
	GAGenome::SexualCrossover scross; // sexual crossover to use
	GAGenome::AsexualCrossover across; // asexual crossover to use
};

#endif
