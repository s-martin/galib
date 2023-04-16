/* ----------------------------------------------------------------------------
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

#include "GABaseGA.h"

/** Simple genetic algorithm class.
 * 
 * 
 */
class GASimpleGA : public GAGeneticAlgorithm
{
  public:
	GADefineIdentity("GASimpleGA", GAID::SimpleGA);

	static GAParameterList &registerDefaultParameters(GAParameterList &);

  public:
	explicit GASimpleGA(const GAGenome &);
	explicit GASimpleGA(const GAPopulation &);
	GASimpleGA(const GASimpleGA &);
	GASimpleGA &operator=(const GASimpleGA &);
	~GASimpleGA() override;
	void copy(const GAGeneticAlgorithm &) override;

	void initialize(unsigned int seed = 0) override;
	void step() override;
	GASimpleGA &operator++()
	{
		step();
		return *this;
	}

	int setptr(const std::string &name, const void *value) override;
	int get(const char *name, void *value) const override;

	bool elitist() const { return el; }
	bool elitist(bool flag)
	{
		params.set(gaNelitism, static_cast<int>(flag));
		return el = flag;
	}

	int minimaxi() const override { return minmax; }
	int minimaxi(int m) override;

	const GAPopulation &population() const override { return *pop; }
	const GAPopulation &population(const GAPopulation &) override;
	int populationSize() const override { return pop->size(); }
	int populationSize(unsigned int n) override;
	GAScalingScheme &scaling() const override { return pop->scaling(); }
	GAScalingScheme &scaling(const GAScalingScheme &s) override
	{
		oldPop->scaling(s);
		return GAGeneticAlgorithm::scaling(s);
	}
	GASelectionScheme &selector() const override { return pop->selector(); }
	GASelectionScheme &selector(const GASelectionScheme &s) override
	{
		oldPop->selector(s);
		return GAGeneticAlgorithm::selector(s);
	}
	void objectiveFunction(GAGenome::Evaluator f) override;
	void objectiveData(const GAEvalData &v) override;

  protected:
	GAPopulation *oldPop; // current and old populations
	bool el; // are we elitist?
};

inline std::ostream &operator<<(std::ostream &os, GASimpleGA &arg)
{
	arg.write(os);
	return (os);
}

inline std::istream &operator>>(std::istream &is, GASimpleGA &arg)
{
	arg.read(is);
	return (is);
}
