// $Header$
/* ----------------------------------------------------------------------------
  gainc.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

  Header file for the incremental genetic algorithm class.

replacement
  The replacement strategy defines how the new children will be stuck into the
population.  If you want the new child to replace one of its parents, use
the Parent strategy.  If you want the child to replace a random population
member, use the Random strategy.  If you want the child to replace the worst
population member, use the Worst strategy.  These are meaningful only for
overlapping populations.  To do DeJong-style speciation (crowding), use the
Crowding strategy.  You must also specify a crowding function as the 
replacement function if you choose this strategy.  If you use Custom as the
replacement strategy you must also specify a replacement function.
  Note that not every replacement scheme can be used with every type of
genetic algorithm.  If a GA supports replacement schemes, it will specify
which schemes are valid and which are not.
  The replacement function is required for crowding and custom replacement
strategies.  This function is used to pick which genome will be 
replaced.  The first argument passed to the replacement function is the
individual that is supposed to go into the population.  The second argument
is the population into which the individual is supposed to go.
The replacement function should return a reference to the genome that the
individual should replace.  If no replacement should take place, the
replacement function should return a reference to the individual.
---------------------------------------------------------------------------- */
#ifndef _ga_gainc_h_
#define _ga_gainc_h_

#include <GABaseGA.h>

class GAIncrementalGA : public GAGeneticAlgorithm {
public:
  GADefineIdentity("GAIncrementalGA", GAID::IncrementalGA);

  using ReplacementFunction = GAGenome &(*)(GAGenome &, GAPopulation &);

  enum ReplacementScheme {
    RANDOM = GAPopulation::RANDOM,
    BEST = GAPopulation::BEST,
    WORST = GAPopulation::WORST,
    CUSTOM = -30,
    CROWDING = -30,
    PARENT = -10
    };

  static GAParameterList& registerDefaultParameters(GAParameterList&);

public:
  explicit GAIncrementalGA(const GAGenome&);
  explicit GAIncrementalGA(const GAPopulation&);
  GAIncrementalGA(const GAIncrementalGA&);
  GAIncrementalGA& operator=(const GAIncrementalGA&);
  ~GAIncrementalGA() override;
  void copy(const GAGeneticAlgorithm &) override;

  void initialize(unsigned int seed=0) override;
  void step() override;
  GAIncrementalGA & operator++() { step(); return *this; }

  int setptr(const std::string &name, const void* value) override;
  int get(const char* name, void* value) const override;

  void objectiveFunction(GAGenome::Evaluator f) override;
  void objectiveData(const GAEvalData& v) override;

  int nOffspring() const {return noffspr;}
  int nOffspring(unsigned int);

  ReplacementScheme replacement() const {return rs;}
  ReplacementScheme replacement(ReplacementScheme, ReplacementFunction f=nullptr);

protected:
  GAGenome *child1, *child2;	// children that will be generated each gen
  ReplacementScheme rs;	// replacement strategy
  ReplacementFunction rf;	// (optional) replacement function
  unsigned int noffspr;		// number of children to generate in crossover
};



inline std::ostream & operator<< (std::ostream & os, GAIncrementalGA & arg)
{ arg.write(os); return(os); }
inline std::istream & operator>> (std::istream & is, GAIncrementalGA & arg)
{ arg.read(is); return(is); }

#endif
