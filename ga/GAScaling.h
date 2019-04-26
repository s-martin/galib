// $Header$
/* ----------------------------------------------------------------------------
  scaling.h
  mbwall 10aug94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This is the GAScaling class.  It is used to do scaling on a population.  When
a genome is evaluated, the user's objective function provides an overall
rating for each genome.  The GA is concerned with fitness, not objective
score (unless you do no scaling).  So this object is the container for the
scaled values.
  Examples of scaling include linear scaling, sigma truncation, and power law.
Goldberg's sharing function is also a type of scaling, and it is implemented
here as a unique class.
  The scaling class is designed to be used with a population.  It is stupid -
it does know how to update itself, but it must be told when.
---------------------------------------------------------------------------- */
#ifndef _ga_scaling_h_
#define _ga_scaling_h_

#include <GAGenome.h>
#include <gaconfig.h>
#include <gaid.h>
#include <gatypes.h>

class GAPopulation;

constexpr float gaDefLinearScalingMultiplier = 1.2;
constexpr float gaDefSigmaTruncationMultiplier = 2.0;
constexpr float gaDefPowerScalingFactor = 1.0005;
constexpr float gaDefSharingCutoff = 1.0;

/* ----------------------------------------------------------------------------
Scaling

  The scaling object is used to scale the objective scores of a population to
avoid clustering and premature convergence (among other things).  See golberg
for more about the theory.  This is basically just a container for any data
that the scaling object might need to do its thing.  The simplest scalings
don't store any data.
---------------------------------------------------------------------------- */
class GAScalingScheme : public GAID
{
  public:
	GADefineIdentity("GAScalingScheme", GAID::Scaling);

	GAScalingScheme() = default;
	GAScalingScheme(const GAScalingScheme &s) { copy(s); }
	GAScalingScheme &operator=(const GAScalingScheme &s)
	{
		copy(s);
		return *this;
	}
	~GAScalingScheme() override = default;
	virtual GAScalingScheme *clone() const = 0;
	virtual void copy(const GAScalingScheme &) {}
	virtual void evaluate(const GAPopulation &p) = 0;
};

/* ----------------------------------------------------------------------------
NoScaling
---------------------------------------------------------------------------- */
class GANoScaling : public GAScalingScheme
{
  public:
	GADefineIdentity("GANoScaling", GAID::NoScaling);

	GANoScaling() : GAScalingScheme() {}
	GANoScaling(const GANoScaling &) : GAScalingScheme() {}
	GANoScaling &operator=(const GAScalingScheme &) { return *this; }
	~GANoScaling() override = default;
	GAScalingScheme *clone() const override { return new GANoScaling(*this); }
	void evaluate(const GAPopulation &p) override;
};

/* ----------------------------------------------------------------------------
LinearScaling

  This scaling object does linear scaling as described in goldberg pp 122-124.
---------------------------------------------------------------------------- */
#if USE_LINEAR_SCALING == 1
class GALinearScaling : public GAScalingScheme
{
  public:
	GADefineIdentity("GALinearScaling", GAID::LinearScaling);

	explicit GALinearScaling(float fm = gaDefLinearScalingMultiplier)
	{
		multiplier(fm);
	}
	GALinearScaling(const GALinearScaling &arg) { copy(arg); }
	GALinearScaling &operator=(const GAScalingScheme &arg)
	{
		copy(arg);
		return (*this);
	}
	~GALinearScaling() override = default;
	GAScalingScheme *clone() const override
	{
		return new GALinearScaling(*this);
	}
	void evaluate(const GAPopulation &p) override;
	void copy(const GAScalingScheme &arg) override
	{
		if (&arg != this)
		{
			GAScalingScheme::copy(arg);
			c = (DYN_CAST(const GALinearScaling &, arg)).c;
		}
	}

	float multiplier(float fm);
	float multiplier() const { return c; }

  protected:
	float c; // linear scaling multiplier
};
#endif

/* ----------------------------------------------------------------------------
SigmaTruncationScaling

  This scaling object does sigma truncation as defined in goldberg p124.
---------------------------------------------------------------------------- */
#if USE_SIGMA_TRUNC_SCALING == 1
class GASigmaTruncationScaling : public GAScalingScheme
{
  public:
	GADefineIdentity("GASigmaTruncationScaling", GAID::SigmaTruncationScaling);

	explicit GASigmaTruncationScaling(float m = gaDefSigmaTruncationMultiplier)
	{
		multiplier(m);
	}
	GASigmaTruncationScaling(const GASigmaTruncationScaling &arg) { copy(arg); }
	GASigmaTruncationScaling &operator=(const GAScalingScheme &arg)
	{
		copy(arg);
		return (*this);
	}
	~GASigmaTruncationScaling() override = default;
	GAScalingScheme *clone() const override
	{
		return new GASigmaTruncationScaling(*this);
	}
	void evaluate(const GAPopulation &p) override;
	void copy(const GAScalingScheme &arg) override
	{
		if (&arg != this)
		{
			GAScalingScheme::copy(arg);
			c = (DYN_CAST(const GASigmaTruncationScaling &, arg)).c;
		}
	}

	float multiplier(float fm);
	float multiplier() const { return c; }

  protected:
	float c; // std deviation multiplier
};
#endif

/* ----------------------------------------------------------------------------
PowerLawScaling

  This scaling object does power law scaling as defined in goldberg p124.
---------------------------------------------------------------------------- */
#if USE_POWER_LAW_SCALING == 1
class GAPowerLawScaling : public GAScalingScheme
{
  public:
	GADefineIdentity("GAPowerLawScaling", GAID::PowerLawScaling);

	explicit GAPowerLawScaling(float f = gaDefPowerScalingFactor) { k = f; }
	GAPowerLawScaling(const GAPowerLawScaling &arg) { copy(arg); }
	GAPowerLawScaling &operator=(const GAScalingScheme &arg)
	{
		copy(arg);
		return (*this);
	}
	~GAPowerLawScaling() override = default;
	GAScalingScheme *clone() const override
	{
		return new GAPowerLawScaling(*this);
	}
	void evaluate(const GAPopulation &p) override;
	void copy(const GAScalingScheme &arg) override
	{
		if (&arg != this)
		{
			GAScalingScheme::copy(arg);
			k = (DYN_CAST(const GAPowerLawScaling &, arg)).k;
		}
	}

	float power(float p) { return k = p; }
	float power() const { return k; }

  protected:
	float k; // power scaling factor
};
#endif

/* ----------------------------------------------------------------------------
Sharing

  This scaling object does sharing as described in goldberg p 192.  This
implementation does triangular sharing with the (optional) alpha parameter for
changing the curvature of the sharing range and the (required) sigma parameter
for controlling the range of influence.  If you want a different type of
sharing function, then derive from this class and define a new (virtual)
evaluate method and add whatever member data you need to specify the shape
of your sharing function.
  We use the distance function to scale the objective scores of the
genomes so that if there are many similar genomes in a population
their scores will be decreased, thus giving sub-species a greater chance to
reproduce.
  This sharing function is defined as follows:

					 /
					|  1 - (d(i,j)/sigma) ^ alpha       d(i,j) < sigma
		s(d(i,j)) = |
					|  0                                d(i,j) >= sigma
					 \

  where d is the distance between any two individuals and is defined such
that d(i,j) = 0 means that individuals i and j are identical.  d() has no
upper bound, but it is never negative.
  The alpha controls the shape of the sharing function.  When alpha=1 then
the 'curve' is a straight line.  If alpha is < 1 then the curves are concave,
if alpha is > 1 then the curves are convex.
  If you decide to use this type of sharing, be careful of the sigma value
that you use.  It can make a HUGE difference, depending upon the objective.
  Distance functions are independent of the sharing functions (as defined in
Goldberg, that is).
  A similarity (distance) function is used with the sharing object.  It is a
type of speciation (similar in functionality to DeJong crowding, but this uses
fitness scaling rather than replacement strategy to affect the speciation).
If the genomes are identical, the similarity function should return a value of
0.0, if completely different then return a value of 1.0.  0 means less
diversity means all genomes are the same.
  You can specify whether the scaling should be maximize or minimize based.
If you are maximizing, the scaling will divide the raw score by the scaling
factor.  If you are minimizing, the scaling will multiply the score by the
scaling factor.  (By definition, the scaling factor will always be >= 1.0)
  By default, the scaling object uses the max/min settings that it contains
(so you can set the scaling independently of the GA).  If the scaling's min/max
was not set, then it tries to use the min/max settings in the GA that owns
the population to which the scaling object is attached.  If there is no GA,
then it bases its min/max on the sort order of the population.
  You can set the minimaxi to:

	GA::MINIMIZE - scale by multiplying the raw scores
	GA::MAXIMIZE - scale by dividing the raw scores
	0            - minimize or maximize based upon the GA's settings

*** This should be called TriangularSharing rather than simply Sharing.
---------------------------------------------------------------------------- */
#if USE_SHARING == 1
class GASharing : public GAScalingScheme
{
  public:
	GADefineIdentity("GASharing", GAID::Sharing);

	GASharing(GAGenome::Comparator func, float cut = gaDefSharingCutoff,
			  float a = 1.0)
	{
		N = 0;
		d = (float *)nullptr;
		df = func;
		_sigma = cut;
		_alpha = a;
		_minmax = 0;
	}
	GASharing(float cut = gaDefSharingCutoff, float a = 1.0)
	{
		N = 0;
		d = (float *)nullptr;
		df = nullptr;
		_sigma = cut;
		_alpha = a;
		_minmax = 0;
	}
	GASharing(const GASharing &arg)
	{
		N = 0;
		d = (float *)nullptr;
		copy(arg);
	}
	GASharing &operator=(const GAScalingScheme &arg)
	{
		copy(arg);
		return (*this);
	}
	~GASharing() override { delete[] d; }
	GAScalingScheme *clone() const override { return new GASharing(*this); }
	void copy(const GAScalingScheme &arg) override;
	void evaluate(const GAPopulation &p) override;

	GAGenome::Comparator distanceFunction(GAGenome::Comparator f)
	{
		return df = f;
	}
	GAGenome::Comparator distanceFunction() const { return df; }

	float sigma(float);
	float sigma() const { return _sigma; }

	float alpha(float c) { return _alpha = c; }
	float alpha() const { return _alpha; }

	int minimaxi(int i);
	int minimaxi() const { return _minmax; }

  protected:
	GAGenome::Comparator df; // the user-defined distance function
	unsigned int N; // how many do we have? (n of n-by-n)
	float *d; // the distances for each genome pair
	float _sigma; // absolute cutoff from central point
	float _alpha; // controls the curvature of sharing f
	int _minmax; // should we minimize or maximize?
};
#endif

#endif
