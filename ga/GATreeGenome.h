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
#include <garandom.h>

extern int _GATreeCompare(GANodeBASE* anode, GANodeBASE* bnode);

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
	  GADefineIdentity("GATreeGenome", GAID::TreeGenome);

	static int DestructiveMutator(GAGenome &, float);
	static int SwapNodeMutator(GAGenome &, float);
	
	// This is a rearranging mutation operator with subtree swap.  It does the same
// thing as the rearranging mutator above, but swaps subtrees as well as the
// nodes that are selected.
//   After the mutation the iterator is left at the root of the tree.
//   We check to make sure that we don't try to swap ancestral nodes.  If it is
// an ancestral swap, we give up and don't do anything to the tree.  This could
// result in mutation rates that are lower than the specified rate!
// *** mutation rates are not exact!
	static int SwapSubtreeMutator(GAGenome& c, float pmut)
	{
		GATreeGenome<T>& child = DYN_CAST(GATreeGenome<T>&, c);

		if (pmut <= 0.0)
			return 0;

		int n = child.size();
		float nMut = pmut * STA_CAST(float, n);
		nMut *= 0.5; // swapping one node swaps another as well
		if (nMut < 1.0)
		{ // we have to do a flip test for each node
			nMut = 0;
			for (int i = 0; i < n; i++)
			{
				if (GAFlipCoin(pmut))
				{
					int b = GARandomInt(0, n - 1);
					if (!child.ancestral(i, b))
						child.swaptree(i, b);
					nMut++;
				}
			}
		}
		else
		{ // only nuke the number of nodes we need to
			for (int i = 0; i < nMut; i++)
			{
				int a = GARandomInt(0, n - 1);
				int b = GARandomInt(0, n - 1);
				if (!child.ancestral(a, b))
					child.swaptree(a, b);
			}
		}
		child.root(); // set iterator to root node

		return (STA_CAST(int, nMut * 2));
	}


	static int OnePointCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								 GAGenome *);
	static float TopologyComparator(const GAGenome &, const GAGenome &);
	//  static float NodeComparator(const GAGenome&, const GAGenome&);

  public:


	//explicit GATreeGenome(GAGenome::Evaluator f = nullptr, void *u = nullptr);
	
	GATreeGenome(GAGenome::Evaluator f = nullptr, void* u = nullptr)
		: GATree<T>(), GAGenome(DEFAULT_TREE_INITIALIZER, DEFAULT_TREE_MUTATOR,
			DEFAULT_TREE_COMPARATOR)
	{
		evaluator(f);
		userData(u);
		crossover(DEFAULT_TREE_CROSSOVER);
	}
	
	GATreeGenome(const GATreeGenome<T> &);
	GATreeGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	//~GATreeGenome() override;
	~GATreeGenome() {};

	GAGenome* clone(GAGenome::CloneMethod flag = CloneMethod::CONTENTS) const override
	{
		GATreeGenome<T>* cpy = new GATreeGenome<T>();
		if (flag == CloneMethod::CONTENTS)
		{
			cpy->copy(*this);
		} // cast is for metrowerks...
		else
		{
			cpy->GAGenome::copy(*this);
		}
		return cpy;
	}

	void copy(const GAGenome& orig) override
	{
		if (&orig == this)
			return;
		const GATreeGenome<T>* c = DYN_CAST(const GATreeGenome<T>*, &orig);
		if (c)
		{
			GAGenome::copy(*c);
			GATree<T>::copy(*c);
		}
	}


	int write(std::ostream &) const override;

	bool equal(const GAGenome &c) const override
	{
		if (this == &c)
			return true;
		const GATreeGenome<T>& b = DYN_CAST(const GATreeGenome<T>&, c);
		return _GATreeCompare(this->rt, b.rt) ? false : true;
	}

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
