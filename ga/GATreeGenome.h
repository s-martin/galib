// $Header$
/* ----------------------------------------------------------------------------
  tree.h
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This header defines the interface for the tree genome.
---------------------------------------------------------------------------- */
#ifndef _ga_tree_h_
#define _ga_tree_h_

#include <GAGenome.h>
#include <GATree.h>

/* ----------------------------------------------------------------------------
TreeGenome
-------------------------------------------------------------------------------
  Beware that the tree genome can grow unbounded - there is no size limit
on the tree, so if you have an objective function that encourages size the tree
will grow until you run out of memory.
---------------------------------------------------------------------------- */
template <class T> class GATreeGenome : public GATree<T>, public GAGenome
{
  public:
	GADeclareIdentity();

	static int DestructiveMutator(GAGenome &, float);
	static int SwapNodeMutator(GAGenome &, float);
	static int SwapSubtreeMutator(GAGenome &, float);
	static int OnePointCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								 GAGenome *);
	static float TopologyComparator(const GAGenome &, const GAGenome &);
	//  static float NodeComparator(const GAGenome&, const GAGenome&);

  public:
	GATreeGenome(GAGenome::Evaluator f = nullptr, void *u = nullptr);
	GATreeGenome(const GATreeGenome<T> &);
	GATreeGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	~GATreeGenome() override;
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
		return GATree<T>::destroy();
	}
	int swaptree(GATree<T> *t)
	{
		_evaluated = false;
		return GATree<T>::swaptree(t);
	}
	int swaptree(unsigned int i, unsigned int j)
	{
		_evaluated = false;
		return GATree<T>::swaptree(i, j);
	}
	int swap(unsigned int i, unsigned int j)
	{
		_evaluated = false;
		return GATree<T>::swap(i, j);
	}
	GATree<T> *remove()
	{
		_evaluated = false;
		return GATree<T>::remove();
	}
	int insert(GATree<T> *t, GATreeBASE::Location where = GATreeBASE::BELOW)
	{
		_evaluated = false;
		return GATree<T>::insert(t, where);
	}
	int insert(const T &t, GATreeBASE::Location where = GATreeBASE::BELOW)
	{
		_evaluated = false;
		return GATree<T>::insert(t, where);
	}
};

#endif
