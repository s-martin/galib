/* ----------------------------------------------------------------------------
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

#include "GABaseGA.h"

/** Steady-state genetic algorithm class.
 *
 * 
 */
class GASteadyStateGA : public GAGeneticAlgorithm
{
  public:
	GADefineIdentity("GASteadyStateGA", GAID::SteadyStateGA);

	static GAParameterList &registerDefaultParameters(GAParameterList &);

  public:
	explicit GASteadyStateGA(const GAGenome &);
	explicit GASteadyStateGA(const GAPopulation &);
	GASteadyStateGA(const GASteadyStateGA &);
	GASteadyStateGA &operator=(const GASteadyStateGA &);
	~GASteadyStateGA() override;
	void copy(const GAGeneticAlgorithm &) override;

	void initialize(unsigned int seed = 0) override;
	void step() override;
	GASteadyStateGA &operator++()
	{
		step();
		return *this;
	}

	int setptr(const std::string &name, const void *value) override;
	int get(const char *name, void *value) const override;

	int minimaxi() const override { return minmax; }
	int minimaxi(int m) override;

	const GAPopulation &population() const override { return *pop; }
	const GAPopulation &population(const GAPopulation &) override;
	int populationSize() const override { return pop->size(); }
	int populationSize(unsigned int n) override;
	GAScalingScheme &scaling() const override { return pop->scaling(); }
	GAScalingScheme &scaling(const GAScalingScheme &s) override
	{ /* tmpPop->scaling(s); */
		return GAGeneticAlgorithm::scaling(s);
	}
	GASelectionScheme &selector() const override { return pop->selector(); }
	GASelectionScheme &selector(const GASelectionScheme &s) override
	{ /* tmpPop->selector(s); */
		return GAGeneticAlgorithm::selector(s);
	}
	void objectiveFunction(GAGenome::Evaluator f) override;
	void objectiveData(const GAEvalData &v) override;

	float pReplacement() const { return pRepl; }
	float pReplacement(float p);
	int nReplacement() const { return nRepl; }
	int nReplacement(unsigned int n);

  protected:
	GAPopulation *tmpPop; // temporary population for replacements
	float pRepl; // percentage of population to replace each gen
	unsigned int nRepl; // how many of each population to replace
	short which; // 0 if prepl, 1 if nrepl
};

inline std::ostream &operator<<(std::ostream &os, GASteadyStateGA &arg)
{
	arg.write(os);
	return (os);
}
inline std::istream &operator>>(std::istream &is, GASteadyStateGA &arg)
{
	arg.read(is);
	return (is);
}
