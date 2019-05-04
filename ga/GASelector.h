// $Header$
/* ----------------------------------------------------------------------------
  selector.h
  mbwall 10aug94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  The selectors are functions that use information in fitness objects to pick
genomes from a population.  There are a number of selection criteria, and
not every selection method will work with every fitness object.

  These are the pre-defined selection methods.  A brief outline of what each
one does is given here, but for more details (especially the theoretical
foundations for each method), see Goldberg.  See the source file for some
implementation details that Goldberg does not delve into.
  The selection methods can, to some extent, be mixed and matched with the
various scaling methods.
  If you want to do speciating selection (e.g. DeJong-style crowding with
Hamming distance) then you will have to write your own selection method.

		 Rank - pick the genome with the best fitness (not objective score)
RouletteWheel - weighted selection where individuals with better fitness have
				a greater chance of being selected than those with lower
				scores.
   Tournament - similar to roulette, but instead of choosing one, choose two
				then pick the better of the two as the selected individuals
	  Uniform - stochastic uniform selection picks randomly from the
				population.  Each individual has as much chance as any other.
		  SRS - stochastic remainder selection does a preselection based on
				the expected number of each genome, then a random sampling on
				the preselected list.
		   DS - deterministic sampling is implemented as described in
				Goldberg's book (as much as I could understand it, anyway).
---------------------------------------------------------------------------- */
#ifndef _ga_selector_h_
#define _ga_selector_h_

#include <gaid.h>
#include <cstring>
#include <vector>

class GAGenome;
class GAPopulation;

/* ----------------------------------------------------------------------------
   The base class definition for the selector object defines the interface.
Any derived selector must define a clone member and a select member.  If you
add any special data members then you should also define a copy member.
   Any selector can do its business based on fitness or objective scores.  The
base selector provides the mechanism for this.  Derived classes can use it if
they want to, or ignore it.
---------------------------------------------------------------------------- */
class GASelectionScheme : public GAID
{
  public:
	GADefineIdentity("GASelectionScheme", GAID::Selection);
	enum
	{
		RAW,
		SCALED
	};

	explicit GASelectionScheme(int w = SCALED) { which = w; }
	GASelectionScheme(const GASelectionScheme &orig) { copy(orig); }
	GASelectionScheme &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GASelectionScheme() override = default;
	virtual GASelectionScheme *clone() const = 0;
	virtual void copy(const GASelectionScheme &orig)
	{
		pop = orig.pop;
		which = orig.which;
	}
	virtual void assign(GAPopulation &p) { pop = &p; }
	virtual void update() {}
	virtual GAGenome &select() const = 0;

  protected:
	GAPopulation *pop;
	int which; // should we use fitness or objective scores?
};

/* ----------------------------------------------------------------------------
   The rank selector simply picks the best individual in the population.  You
can specify whether the selector should use the raw (objective) scores or the
scaled (fitness) scores to determine the best individual.  Default is fitness.
---------------------------------------------------------------------------- */
#if USE_RANK_SELECTOR == 1
class GARankSelector : public GASelectionScheme
{
  public:
	GADefineIdentity("GARankSelector", GAID::RankSelection);

	explicit GARankSelector(int w = GASelectionScheme::SCALED)
		: GASelectionScheme(w)
	{
	}
	GARankSelector(const GARankSelector &orig) : GASelectionScheme(orig)
	{
		copy(orig);
	}
	GARankSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GARankSelector() override = default;
	GASelectionScheme *clone() const override
	{
		return new GARankSelector;
	}
	GAGenome &select() const override;
};
#endif

/* ----------------------------------------------------------------------------
   Roulette wheel uses a fitness-proportional algorithm for selecting
individuals.
---------------------------------------------------------------------------- */
#if USE_ROULETTE_SELECTOR == 1 || USE_TOURNAMENT_SELECTOR == 1
class GARouletteWheelSelector : public GASelectionScheme
{
  public:
	GADefineIdentity("GARouletteWheelSelector", GAID::RouletteWheelSelection);

	explicit GARouletteWheelSelector(int w = GASelectionScheme::SCALED)
		: GASelectionScheme(w)
	{
		psum.clear();
		n = 0;
	}
	GARouletteWheelSelector(const GARouletteWheelSelector &orig)
		: GASelectionScheme(orig)
	{
		psum.clear();
		n = 0;
		copy(orig);
	}
	GARouletteWheelSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GARouletteWheelSelector() override { }
	GASelectionScheme *clone() const override
	{
		return new GARouletteWheelSelector;
	}
	void copy(const GASelectionScheme &orig) override
	{
		GASelectionScheme::copy(orig);
		const GARouletteWheelSelector &sel =
			DYN_CAST(const GARouletteWheelSelector &, orig);
		psum = sel.psum;
	}
	GAGenome &select() const override;
	void update() override;

  protected:
	int n;
	std::vector<float> psum;
};
#endif

/* ----------------------------------------------------------------------------
   This version of the tournament selector does two roulette wheel selections
then picks the better of the two.  We derive from the roulette wheel class so
that we can use its update method.
---------------------------------------------------------------------------- */
#if USE_TOURNAMENT_SELECTOR == 1
class GATournamentSelector : public GARouletteWheelSelector
{
  public:
	GADefineIdentity("GATournamentSelector", GAID::TournamentSelection);

	explicit GATournamentSelector(int w = GASelectionScheme::SCALED)
		: GARouletteWheelSelector(w)
	{
	}
	GATournamentSelector(const GATournamentSelector &orig)
		: GARouletteWheelSelector(orig)
	{
		copy(orig);
	}
	GATournamentSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GATournamentSelector() override = default;
	GASelectionScheme *clone() const override
	{
		return new GATournamentSelector;
	}
	GAGenome &select() const override;
};
#endif

/* ----------------------------------------------------------------------------
   Stochastic uniform sampling selection.  This is just a fancy name for
random sampling.  Any individual in the population has as much chance of being
selected as any other.
---------------------------------------------------------------------------- */
#if USE_UNIFORM_SELECTOR == 1
class GAUniformSelector : public GASelectionScheme
{
  public:
	GADefineIdentity("GAUniformSelector", GAID::UniformSelection);

	explicit GAUniformSelector(int w = GASelectionScheme::SCALED)
		: GASelectionScheme(w)
	{
	}
	GAUniformSelector(const GAUniformSelector &orig) : GASelectionScheme(orig)
	{
		copy(orig);
	}
	GAUniformSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GAUniformSelector() override = default;
	GASelectionScheme *clone() const override
	{
		return new GAUniformSelector;
	}
	GAGenome &select() const override;
};
#endif

/* ----------------------------------------------------------------------------
   Stochastic remainder sampling selection.
---------------------------------------------------------------------------- */
#if USE_SRS_SELECTOR == 1
class GASRSSelector : public GASelectionScheme
{
  public:
	GADefineIdentity("GASRSSelector", GAID::SRSSelection);

	explicit GASRSSelector(int w = GASelectionScheme::SCALED)
		: GASelectionScheme(w)
	{
		n = 0;
	}
	GASRSSelector(const GASRSSelector &orig) : GASelectionScheme(orig)
	{
		n = 0;
		copy(orig);
	}
	GASRSSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GASRSSelector() override
	{
	}
	GASelectionScheme *clone() const override
	{
		return new GASRSSelector;
	}
	void copy(const GASelectionScheme &orig) override
	{
		GASelectionScheme::copy(orig);
		const GASRSSelector &sel = DYN_CAST(const GASRSSelector &, orig);
		n = sel.n;
		fraction = sel.fraction;
		choices = sel.choices;
	}
	GAGenome &select() const override;
	void update() override;

  protected:
	std::vector<float> fraction;
	std::vector<unsigned int> choices;
	unsigned int n;
};
#endif

/* ----------------------------------------------------------------------------
   Deterministic sampling selection.
---------------------------------------------------------------------------- */
#if USE_DS_SELECTOR == 1
class GADSSelector : public GASelectionScheme
{
  public:
	GADefineIdentity("GADSSelector", GAID::DSSelection);

	explicit GADSSelector(int w = GASelectionScheme::SCALED)
		: GASelectionScheme(w)
	{
		n = 0;
	}
	GADSSelector(const GADSSelector &orig) : GASelectionScheme(orig)
	{
		n = 0;
		copy(orig);
	}
	GADSSelector &operator=(const GASelectionScheme &orig)
	{
		if (&orig != this) {
			copy(orig);
}
		return *this;
	}
	~GADSSelector() override
	{
	}
	GASelectionScheme *clone() const override
	{
		return new GADSSelector;
	}
	void copy(const GASelectionScheme &orig) override
	{
		GASelectionScheme::copy(orig);
		const GADSSelector &sel = DYN_CAST(const GADSSelector &, orig);
		n = sel.n;
		fraction = sel.fraction;
		choices = sel.choices;
		idx = sel.idx;
	}
	GAGenome &select() const override;
	void update() override;

  protected:
	std::vector<float> fraction;
	std::vector<unsigned int> choices;
	std::vector<unsigned int> idx;
	unsigned int n;
};
#endif

#endif
