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

  public:
	GASimpleGA(const GAGenome &, const std::shared_ptr<GAParameterList>& _params);
	GASimpleGA(const GAPopulation &, const std::shared_ptr<GAParameterList>& _params);
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

	bool elitist() const { return el; }
	bool elitist(bool flag)
	{
		params->set(gaNelitism, static_cast<int>(flag));
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

#endif
