#pragma once

#include <GASStateGA.h>
#include <GATreeGenome.hpp>

#include <iostream>


/* ----------------------------------------------------------------------------
Objective function
  All we do in this objective function is try to maximize the size of the tree.
Just return the tree size.  This means that if you run this objective function
for many generations you'll run out of memory!  There is no limit to tree or
list sizes built-in to the GA library.
---------------------------------------------------------------------------- */
float objectiveEx6(GAGenome &c)
{
	GATreeGenome<int> &genome = (GATreeGenome<int> &)c;
	return genome.size();
}

/* ----------------------------------------------------------------------------
Here is the initializer for our genomes.  It builds a tree of n items of type
int.  Notice that we first destroy any tree that is already in the genome
before we do our initialization.  This is so that the genomes can be re-used.
When you re-run a GA, it does not destroy the individuals in the population -
it reuses them.  Thus, the initializer must make sure that the genome is
cleaned up before it tries to initialize it.
---------------------------------------------------------------------------- */
void TreeInitializer(GAGenome &c)
{
	GATreeGenome<int> &child = (GATreeGenome<int> &)c;

	// destroy any pre-existing tree
	child.root();
	child.destroy();

	// Create a new tree with depth of 'depth' and each eldest node containing
	// 'n' children (the other siblings have none).
	int depth = 2, n = 3, count = 0;
	child.insert(count++, GATreeBASE::ROOT);

	for (int i = 0; i < depth; i++)
	{
		child.eldest();
		child.insert(count++);
		for (int j = 0; j < n; j++)
			child.insert(count++, GATreeBASE::AFTER);
	}
}

/* ----------------------------------------------------------------------------
Tree genome method overrides
-------------------------------------------------------------------------------
  Here we override the built-in methods for the tree class.  We can do this
because the tree class is template-ized - when the compiler looks for an
instance of the tree class methods, it finds these so it won't generate an
instance from the templates.  You can do this with ANY method of a template
class.  Here we do it only for the write method.
  The default write operator prints out pointers to the contents of each node.
Here we print out the actual contents of each node.  This assumes that the
object in our node has the operator<< defined for it.
---------------------------------------------------------------------------- */
void WriteNode(std::ostream &os, GANode<int> *n)
{
	if (!n)
		return;
	GANodeBASE *node = (GANodeBASE *)n;

	os.width(10);
	os << ((GANode<int> *)node)->contents << " ";
	os.width(10);
	if (node->parent)
		os << ((GANode<int> *)node->parent)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->child)
		os << ((GANode<int> *)node->child)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->next)
		os << ((GANode<int> *)node->next)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->prev)
		os << ((GANode<int> *)node->prev)->contents << "\n";
	else
		os << ".\n";
	WriteNode(os, (GANode<int> *)node->child);

	for (GANodeBASE *tmp = node->next; tmp && tmp != node; tmp = tmp->next)
	{
		os.width(10);
		os << ((GANode<int> *)tmp)->contents << " ";
		os.width(10);
		if (tmp->parent)
			os << ((GANode<int> *)tmp->parent)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->child)
			os << ((GANode<int> *)tmp->child)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->next)
			os << ((GANode<int> *)tmp->next)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->prev)
			os << ((GANode<int> *)tmp->prev)->contents << "\n";
		else
			os << ".\n";
		WriteNode(os, (GANode<int> *)tmp->child);
	}
}

template <> int GATreeGenome<int>::write(std::ostream &os) const
{
	os << "      node     parent      child       next       prev\n";
	WriteNode(os, (GANode<int> *)rt);
	return os.fail() ? 1 : 0;
}

GATreeGenome<int> ex6(const std::shared_ptr<GAParameterList>& params, unsigned int seed)
{
	// Now create the GA and run it.  We first create a chromsome with the
	// operators we want.  Once we have the genome set up, create the genetic
	// algorithm, set the parameters, and let it go.
	GATreeGenome<int> genome(objectiveEx6);
	genome.initializer(TreeInitializer);
	genome.mutator(GATreeGenome<int>::SwapSubtreeMutator);

	GASteadyStateGA ga(genome, params);
	ga.evolve(seed);

	genome = ga.statistics().bestIndividual();
	//  std::cout << "the ga generated this tree:\n" << genome << "\n";
	std::cout << genome.size() << " nodes, " << genome.depth() << " levels deep.\n";
	std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

	return genome;
}