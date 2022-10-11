/* ----------------------------------------------------------------------------
  gademe.h
  mbwall jan96
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
						  all rights reserved

  Header for the deme (parallel population) genetic algorithm class.
  This genetic algorithm lets you specify a number of individuals to migrate
from one population to another at the end of each generation.  You can specify
how many populations to maintain.  Each population evolves using a steady-state
genetic algorithm.  At the end of each generation, the specified number of
individuals migrate from one population to the next (we use the loop migration
topology in this implementation).
  You can modify the migration method by deriving a new class from this one and
redefine the migration method.  If you want to use a different kind of genetic
algorithm for each population then you'll have to modify the mechanics of the
step method.
---------------------------------------------------------------------------- */
#ifndef _ga_gademe_h_
#define _ga_gademe_h_

#include <GABaseGA.h>

class GADemeGA : public GAGeneticAlgorithm
{
  public:
	GADefineIdentity("GADemeGA", GAID::DemeGA);

	enum
	{
		ALL = (-1)
	};

  public:
	explicit GADemeGA(const GAGenome &, const std::shared_ptr<GAParameterList>& _params);
	explicit GADemeGA(const GAPopulation &, const std::shared_ptr<GAParameterList>& _params);
	GADemeGA(const GADemeGA &);
	GADemeGA &operator=(const GADemeGA &);
	~GADemeGA() override;
	void copy(const GAGeneticAlgorithm &) override;

	void initialize(unsigned int seed = 0) override;
	void step() override;
	virtual void migrate(); // new for this derived class
	GADemeGA &operator++()
	{
		step();
		return *this;
	}

	int minimaxi() const override { return minmax; }
	int minimaxi(int m) override;

	const GAPopulation &population() const override { return *pop; }
	const GAPopulation &population(const GAPopulation &p) override
	{
		GAGeneticAlgorithm::population(p);
		return population(ALL, p);
	}
	int populationSize() const override { return pop->size(); }
	int populationSize(unsigned int n) override
	{
		GAGeneticAlgorithm::populationSize(n);
		return populationSize(ALL, n);
	}
	GAScalingScheme &scaling() const override { return pop->scaling(); }
	GAScalingScheme &scaling(const GAScalingScheme &s) override
	{
		GAGeneticAlgorithm::scaling(s);
		return scaling(ALL, s);
	}
	GASelectionScheme &selector() const override { return pop->selector(); }
	GASelectionScheme &selector(const GASelectionScheme &s) override
	{
		GAGeneticAlgorithm::selector(s);
		return selector(ALL, s);
	}
	void objectiveFunction(GAGenome::Evaluator f) override
	{
		GAGeneticAlgorithm::objectiveFunction(f);
		objectiveFunction(ALL, f);
	}
	void objectiveData(const GAEvalData &v) override
	{
		GAGeneticAlgorithm::objectiveData(v);
		objectiveData(ALL, v);
	}

	const GAPopulation &population(unsigned int i) const { return *deme[i]; }
	const GAPopulation &population(int i, const GAPopulation &);
	int populationSize(unsigned int i) const { return deme[i]->size(); }
	int populationSize(int i, unsigned int n);
	int nReplacement(unsigned int i) const { return nrepl[i]; }
	int nReplacement(int i, unsigned int n);
	int nMigration() const { return nmig; }
	int nMigration(unsigned int i);
	int nPopulations() const { return npop; }
	int nPopulations(unsigned int i);

	GAScalingScheme &scaling(unsigned int i) const
	{
		return deme[i]->scaling();
	}
	GAScalingScheme &scaling(int i, const GAScalingScheme &s);
	GASelectionScheme &selector(unsigned int i) const
	{
		return deme[i]->selector();
	}
	GASelectionScheme &selector(int i, const GASelectionScheme &s);
	void objectiveFunction(int i, GAGenome::Evaluator f);
	void objectiveData(int i, const GAEvalData &);

	const GAStatistics &statistics() const { return stats; }
	const GAStatistics &statistics(unsigned int i) const { return pstats[i]; }

  protected:
	unsigned int npop; // how many populations do we have?
	int *nrepl; // how many to replace each generation
	GAPopulation **deme; // array of populations that we'll use
	GAPopulation *tmppop; // temp pop for doing the evolutions
	GAStatistics *pstats; // statistics for each population
	unsigned int nmig; // number to migrate from each population
};

inline std::ostream &operator<<(std::ostream &os, GADemeGA &arg)
{
	arg.write(os);
	return (os);
}
inline std::istream &operator>>(std::istream &is, GADemeGA &arg)
{
	arg.read(is);
	return (is);
}

#endif
