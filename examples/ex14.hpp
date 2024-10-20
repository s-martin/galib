#pragma once

#include <ga.h>
#include <iostream>

// Here we specify how big the lists will be and how many lists will be in each
// composite genome.  These are the default values - you can change them from
// the command line.  Beware that this program will break if nrobots is bigger
// than the size of the lists.
int listsize = 10;
int nrobots = 6;

// This is the class definition.  We override the methods of the base class and
// define a few methods of our own to access the protected members.  The list
// genomes in the composite genome are assigned the 'List' operators
// by default (they can be overridden by using the 'Operator' members on the
// lists explicitly).
//   The ID can be any number over 200 (IDs under 200 are reserved for use by
// GAlib objects).
class RobotPathGenome : public GAGenome
{
  public:
	GADefineIdentity("RobotPathGenome", 251);
	static void Initializer(GAGenome &);
	static int Mutator(GAGenome &, float);
	static float Comparator(const GAGenome &, const GAGenome &);
	static float Evaluator(GAGenome &);
	static void PathInitializer(GAGenome &);
	static int Crossover(const GAGenome &, const GAGenome &, GAGenome *,
						 GAGenome *);

  public:
	RobotPathGenome(int noofrobots, int pathlength);
	RobotPathGenome(const RobotPathGenome &orig)
	 : GAGenome(orig) {
		n = l = 0;
		list = nullptr;
		copy(orig);
	}
	RobotPathGenome operator=(const GAGenome &arg)
	{
		copy(arg);
		return *this;
	}
	~RobotPathGenome() override;
	GAGenome *clone(GAGenome::CloneMethod) const override;
	void copy(const GAGenome &c) override;
	bool equal(const GAGenome &g) const override;
	int read(std::istream &is) override;
	int write(std::ostream &os) const override;

	GAListGenome<int> &path(const int i) { return *list[i]; }
	int npaths() const { return n; }
	int length() const { return l; }

  protected:
	int n, l;
	GAListGenome<int> **list;
};

RobotPathGenome::RobotPathGenome(int noofrobots, int pathlength)
	: GAGenome(Initializer, Mutator, Comparator)
{
	evaluator(Evaluator);
	crossover(Crossover);
	n = noofrobots;
	l = pathlength;
	list = (n ? new GAListGenome<int> *[n] : (GAListGenome<int> **)nullptr);
	for (int i = 0; i < n; i++)
	{
		list[i] = new GAListGenome<int>;
		list[i]->initializer(PathInitializer);
		list[i]->mutator(GAListGenome<int>::SwapMutator);
	}
}

void RobotPathGenome::copy(const GAGenome &g)
{
	if (&g != this && sameClass(g))
	{
		GAGenome::copy(g); // copy the base class part
		auto &genome = (RobotPathGenome &)g;
		if (n == genome.n)
		{
			for (int i = 0; i < n; i++)
				list[i]->copy(*genome.list[i]);
		}
		else
		{
			int i;
			for (i = 0; i < n; i++)
				delete list[i];
			delete[] list;
			n = genome.n;
			list = new GAListGenome<int> *[n];
			for (i = 0; i < n; i++)
				list[i] = (GAListGenome<int> *)genome.list[i]->clone();
		}
	}
}

RobotPathGenome::~RobotPathGenome()
{
	for (int i = 0; i < n; i++)
		delete list[i];
	delete[] list;
}

GAGenome *RobotPathGenome::clone(GAGenome::CloneMethod) const
{
	return new RobotPathGenome(*this);
}

bool RobotPathGenome::equal(const GAGenome &g) const
{
	auto &genome = (RobotPathGenome &)g;
	bool flag = false;
	for (int i = 0; i < n && flag == 0; i++)
		flag = list[i]->equal(*genome.list[i]);
	return flag;
}

int RobotPathGenome::read(std::istream &is)
{
	for (int i = 0; i < n; i++)
		is >> *(list[i]);
	return is.fail() ? 1 : 0;
}

int RobotPathGenome::write(std::ostream &os) const
{
	for (int i = 0; i < n; i++)
		os << "list " << i << ":\t" << *(list[i]) << "\n";
	return os.fail() ? 1 : 0;
}

// These are the definitions of the operators for the robot path genome.
void RobotPathGenome::Initializer(GAGenome &g)
{
	auto &genome = (RobotPathGenome &)g;
	for (int i = 0; i < genome.npaths(); i++)
		genome.path(i).initialize();
	genome._evaluated = false;
}

int RobotPathGenome::Mutator(GAGenome &g, float pmut)
{
	auto &genome = (RobotPathGenome &)g;
	int nMut = 0;
	for (int i = 0; i < genome.npaths(); i++)
		nMut += genome.path(i).mutate(pmut);
	if (nMut)
		genome._evaluated = false;
	return nMut;
}

float RobotPathGenome::Comparator(const GAGenome &a, const GAGenome &b)
{
	auto &sis = (RobotPathGenome &)a;
	auto &bro = (RobotPathGenome &)b;
	float diff = 0;
	for (int i = 0; i < sis.npaths(); i++)
		diff += sis.path(i).compare(bro.path(i));
	return diff / sis.npaths();
}

// The objective function should evaluate the genomes.  This one tries to
// put the node with value 0 into the nth position where n is the number of the
// list in the composite genome.  We're assuming that there are more nodes
// in the list than there are lists in the composite genome.
float RobotPathGenome::Evaluator(GAGenome &c)
{
	auto &genome = (RobotPathGenome &)c;
	float score = 0;
	for (int i = 0; i < genome.npaths(); i++)
		if (*genome.path(i).warp(i) == 0)
			score += 1;
	return score;
}

// This crossover method assumes that all of the robot path genomes have the
// same number of paths in them.  With a few modifications you could make the
// paths be variable length, but then you must use a crossover method other
// than the partial match crossover used here (defined in the robot path
// crossover object).
int RobotPathGenome::Crossover(const GAGenome &a, const GAGenome &b,
							   GAGenome *c, GAGenome *d)
{
	auto &mom = (RobotPathGenome &)a;
	auto &dad = (RobotPathGenome &)b;

	int n = 0;
	if (c && d)
	{
		auto &sis = (RobotPathGenome &)*c;
		auto &bro = (RobotPathGenome &)*d;
		for (int i = 0; i < mom.npaths(); i++)
			GAListGenome<int>::PartialMatchCrossover(
				mom.path(i), dad.path(i), &sis.path(i), &bro.path(i));
		sis._evaluated = false;
		bro._evaluated = false;
		n = 2;
	}
	else if (c)
	{
		auto &sis = (RobotPathGenome &)*c;
		for (int i = 0; i < mom.npaths(); i++)
			GAListGenome<int>::PartialMatchCrossover(mom.path(i), dad.path(i),
													 &sis.path(i), nullptr);
		sis._evaluated = false;
		n = 1;
	}
	else if (d)
	{
		auto &bro = (RobotPathGenome &)*d;
		for (int i = 0; i < mom.npaths(); i++)
			GAListGenome<int>::PartialMatchCrossover(mom.path(i), dad.path(i),
													 nullptr, &bro.path(i));
		bro._evaluated = false;
		n = 1;
	}
	return n;
}

// This is the initialization operator for our lists.  We create a list that is
// n long and whose nodes contain numbers in sequence.
//   The first thing to do in the initializer is to clear out any old
// contents - we do not want to build our new list on a previously existing
// one!  Notice that we have to cast the genome into the type of
// genome we're using (in this case a list).  The GA always operates on
// generic genomes.
//   All of our lists must be the same length since we're going to use the
// ordered crossover operators.
void RobotPathGenome::PathInitializer(GAGenome &c)
{
	auto &list = (GAListGenome<int> &)c;

	// We must first destroy any pre-existing list.
	while (list.head())
		list.destroy();

	// Insert the head of the list.  This node has a random number in it, but
	// the number is in a range different than those in the rest of the list.
	// This way we'll be able to see how the lists get scrambled up.  Since
	// we're using ordered crossover (see the header file) we should never end
	// up with more than one node in each list that has a given value.
	list.insert(0, GAListBASE::HEAD);

	// Loop through n times and append nodes onto the end of the list.
	int i;
	for (i = 0; i < listsize - 1; i++)
		list.insert(i + 20, GAListBASE::AFTER);

	// Now randomize the contents of the list.
	for (i = 0; i < listsize; i++)
		if (GARandomBit())
			list.swap(i, GARandomInt(0, listsize - 1));
}

// Here is the specialization of the write method for our lists.  The default
// write method just prints out pointers to the contents of the nodes (it has
// no way of knowing in advance how you'll want things printed).  Here we
// do almost the same thing, but print out the contents of the nodes rather
// than the pointers to the contents.
template <> int GAListGenome<int>::write(std::ostream &os) const
{
	int *cur, *head;
	GAListIter<int> itertmp(*this);
	if ((head = itertmp.head()) != nullptr)
		os << *head << " ";
	for (cur = itertmp.next(); cur && cur != head; cur = itertmp.next())
		os << *cur << " ";
	return os.fail() ? 1 : 0;
}

float objective(GAGenome &c)
{
	return RobotPathGenome::Evaluator(c);
}

GAStatistics example14(unsigned int seed, bool useStatic)
{
	RobotPathGenome genome(nrobots, listsize);
	GASteadyStateGA ga(genome);
	ga.evolve(seed);

	genome.initialize();
	std::cout << "a randomly-generated set of paths:\n" << genome << std::endl;
	std::cout << "the ga generated:\n"
			  << ga.statistics().bestIndividual() << "\n";

	return ga.statistics();
}
