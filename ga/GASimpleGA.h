// $Header$
/* ----------------------------------------------------------------------------
  gasimple.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

  Header file for the simple genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_gasimple_h_
#define _ga_gasimple_h_

#include <GABaseGA.h>

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
	virtual ~GASimpleGA();
	virtual void copy(const GAGeneticAlgorithm &) override;

	virtual void initialize(unsigned int seed = 0) override;
	virtual void step() override;
	GASimpleGA &operator++()
	{
		step();
		return *this;
	}

	virtual int setptr(const char *name, const void *value) override;
	virtual int get(const char *name, void *value) const override;

	bool elitist() const { return el; }
	bool elitist(bool flag)
	{
		params.set(gaNelitism, (int)flag);
		return el = flag;
	}

	virtual int minimaxi() const override { return minmax; }
	virtual int minimaxi(int m) override;

	virtual const GAPopulation &population() const override { return *pop; }
	virtual const GAPopulation &population(const GAPopulation &) override;
	virtual int populationSize() const override { return pop->size(); }
	virtual int populationSize(unsigned int n) override;
	virtual GAScalingScheme &scaling() const override { return pop->scaling(); }
	virtual GAScalingScheme &scaling(const GAScalingScheme &s) override
	{
		oldPop->scaling(s);
		return GAGeneticAlgorithm::scaling(s);
	}
	virtual GASelectionScheme &selector() const override { return pop->selector(); }
	virtual GASelectionScheme &selector(const GASelectionScheme &s) override
	{
		oldPop->selector(s);
		return GAGeneticAlgorithm::selector(s);
	}
	virtual void objectiveFunction(GAGenome::Evaluator f) override;
	virtual void objectiveData(const GAEvalData &v) override;

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

#endif
