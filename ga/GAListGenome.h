// $Header$
/* ----------------------------------------------------------------------------
  list.h
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This header defines the interface for the list genome.
---------------------------------------------------------------------------- */
#ifndef _ga_list_h_
#define _ga_list_h_

#include <GAGenome.h>
#include <GAList.h>

/* ----------------------------------------------------------------------------
ListGenome
-------------------------------------------------------------------------------
---------------------------------------------------------------------------- */
template <class T> class GAListGenome : public GAList<T>, public GAGenome
{
  public:
	GADeclareIdentity();

	static int DestructiveMutator(GAGenome &, float);
	static int SwapMutator(GAGenome &, float);
	static float NodeComparator(const GAGenome &, const GAGenome &);
	static int OnePointCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								 GAGenome *);
	static int PartialMatchCrossover(const GAGenome &, const GAGenome &,
									 GAGenome *, GAGenome *);
	static int OrderCrossover(const GAGenome &, const GAGenome &, GAGenome *,
							  GAGenome *);
	static int CycleCrossover(const GAGenome &, const GAGenome &, GAGenome *,
							  GAGenome *);

  public:
	explicit GAListGenome(GAGenome::Evaluator f = nullptr, void *u = nullptr);
	GAListGenome(const GAListGenome<T> &);
	GAListGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	~GAListGenome() override;
	GAGenome *clone(GAGenome::CloneMethod flag = CONTENTS) const override;
	void copy(const GAGenome &) override;

	int write(std::ostream &) const override;

	bool equal(const GAGenome &c) const override;

	// Here we do inlined versions of the access members of the super class.  We
	// do our own here so that we can set/unset the _evaluated flag
	// appropriately.

	int destroy()
	{
		_evaluated = false;
		return GAList<T>::destroy();
	}
	int swap(unsigned int i, unsigned int j)
	{
		_evaluated = false;
		return GAList<T>::swap(i, j);
	}
	T *remove()
	{
		_evaluated = false;
		return GAList<T>::remove();
	}
	int insert(GAList<T> *t, GAListBASE::Location where = GAListBASE::AFTER)
	{
		_evaluated = false;
		return GAList<T>::insert(t, where);
	}
	int insert(const T &t, GAListBASE::Location where = GAListBASE::AFTER)
	{
		_evaluated = false;
		return GAList<T>::insert(t, where);
	}
};

#endif
