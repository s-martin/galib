// $Header$
/* ----------------------------------------------------------------------------
  dcrowdingga.h
  mbwall 29mar99
  Copyright (c) 1999 Matthew Wall, all rights reserved

  Header file for the steady-state genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_deterministic_crowding_ga_h_
#define _ga_deterministic_crowding_ga_h_

#include <GABaseGA.h>

class GADCrowdingGA : public GAGeneticAlgorithm
{
  public:
	GADefineIdentity("GADeterministicCrowdingGA", 241);

	explicit GADCrowdingGA(const GAGenome &g) : GAGeneticAlgorithm(g) {}
	~GADCrowdingGA() override = default;

	void initialize(unsigned int seed = 0) override;
	void step() override;
	GADCrowdingGA &operator++()
	{
		step();
		return *this;
	}
};

#endif
