// $Header$
/* ----------------------------------------------------------------------------
  gasteadystate.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

  Header file for the steady-state genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_gasteadystate_h_
#define _ga_gasteadystate_h_

#include <GABaseGA.h>

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
	virtual ~GASteadyStateGA();
	virtual void copy(const GAGeneticAlgorithm &) override;

	virtual void initialize(unsigned int seed = 0) override;
	virtual void step() override;
	GASteadyStateGA &operator++()
	{
		step();
		return *this;
	}

	virtual int setptr(const char *name, const void *value) override;
	virtual int get(const char *name, void *value) const override;

	virtual int minimaxi() const override { return minmax; }
	virtual int minimaxi(int m) override;

	virtual const GAPopulation &population() const override { return *pop; }
	virtual const GAPopulation &population(const GAPopulation &) override;
	virtual int populationSize() const override { return pop->size(); }
	virtual int populationSize(unsigned int n) override;
	virtual GAScalingScheme &scaling() const override { return pop->scaling(); }
	virtual GAScalingScheme &scaling(const GAScalingScheme &s) override
	{ /* tmpPop->scaling(s); */
		return GAGeneticAlgorithm::scaling(s);
	}
	virtual GASelectionScheme &selector() const override { return pop->selector(); }
	virtual GASelectionScheme &selector(const GASelectionScheme &s) override
	{ /* tmpPop->selector(s); */
		return GAGeneticAlgorithm::selector(s);
	}
	virtual void objectiveFunction(GAGenome::Evaluator f) override;
	virtual void objectiveData(const GAEvalData &v) override;

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

#ifdef GALIB_USE_STREAMS
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
#endif

#endif
