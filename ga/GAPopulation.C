// $Header$
/* ----------------------------------------------------------------------------
  population.C
  mbwall 11aug94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */
#include <GABaseGA.h> // for the sake of flaky g++ compiler
#include <GAPopulation.h>
#include <GASelector.h>
#include <garandom.h>
#include <cmath>
#include <cstring>

// windows is promiscuous in its use of min/max, and that causes us grief.  so
// turn of the use of min/max macros in this file.   thanks nick wienholt
#if !defined(NOMINMAX)
#define NOMINMAX
#endif

// This is the default population initializer.  It simply calls the initializer
// for each member of the population.  Then we touch the population to tell it
// that it needs to update stats and/or sort (but we don't actually force
// either one to occur.
//   The population object takes care of setting/unsetting the status flags.
void GAPopulation::DefaultInitializer(GAPopulation &p)
{
	for (int i = 0; i < p.size(); i++) {
		p.individual(i).initialize();
}
}

//  The default evaluator simply calls the evaluate member of each genome in
// the population.  The population object takes care of setting/unsetting the
// status flags for indicating when the population needs to be updated again.
void GAPopulation::DefaultEvaluator(GAPopulation &p)
{
	for (int i = 0; i < p.size(); i++) {
		p.individual(i).evaluate();
}
}

constexpr int GA_POP_CHUNKSIZE = 10; // allocate chrom ptrs in chunks of this many

/* ----------------------------------------------------------------------------
 Population

  The population class is basically just a holder for the genomes.  We also
keep track of statistics about the fitness of our genomes.  We don't care
what kind of genomes we get.  To create the population we call the clone
method of the genome we're given.
  By default we do not calculate the population's diversity, so we set the
div matrix to NULL.
---------------------------------------------------------------------------- */
GAPopulation::GAPopulation()
{
	csz = N = GA_POP_CHUNKSIZE;
	n = 0;
	while (N < n) {
		N += csz;
}

	rind = new GAGenome *[N];
	sind = new GAGenome *[N];
	memset(rind, 0, N * sizeof(GAGenome *));
	memset(sind, 0, N * sizeof(GAGenome *));
	//  indDiv = new float[N*N];
	indDiv = nullptr;

	neval = 0;
	rawSum = rawAve = rawDev = rawVar = rawMax = rawMin = 0.0;
	fitSum = fitAve = fitDev = fitVar = fitMax = fitMin = 0.0;
	popDiv = -1.0;
	rsorted = ssorted = evaluated = false;
	scaled = statted = divved = selectready = false;
	sortorder = HIGH_IS_BEST;
	init = DefaultInitializer;
	eval = DefaultEvaluator;
	slct = new DEFAULT_SELECTOR;
	slct->assign(*this);
	sclscm = new DEFAULT_SCALING;
	evaldata = nullptr;
	ga = nullptr;
}

GAPopulation::GAPopulation(const GAGenome &c, unsigned int popsize)
{
	csz = N = GA_POP_CHUNKSIZE;
	n = (popsize < 1 ? 1 : popsize);
	while (N < n) {
		N += csz;
}

	rind = new GAGenome *[N];
	sind = new GAGenome *[N];
	for (unsigned int i = 0; i < n; i++) {
		rind[i] = c.clone(GAGenome::CloneMethod::ATTRIBUTES);
}
	memcpy(sind, rind, N * sizeof(GAGenome *));
	//  indDiv = new float[N*N];
	indDiv = nullptr;

	neval = 0;
	rawSum = rawAve = rawDev = rawVar = rawMax = rawMin = 0.0;
	fitSum = fitAve = fitDev = fitVar = fitMax = fitMin = 0.0;
	popDiv = -1.0;
	rsorted = ssorted = evaluated = false;
	scaled = statted = divved = selectready = false;
	sortorder = HIGH_IS_BEST;
	init = DefaultInitializer;
	eval = DefaultEvaluator;
	slct = new DEFAULT_SELECTOR;
	slct->assign(*this);
	sclscm = new DEFAULT_SCALING;
	evaldata = nullptr;
	ga = nullptr;
}

GAPopulation::GAPopulation(const GAPopulation &orig)
{
	n = N = 0;
	rind = sind = nullptr;
	indDiv = nullptr;
	sclscm = nullptr;
	slct = nullptr;
	evaldata = nullptr;
	copy(orig);
}

GAPopulation::~GAPopulation()
{
	for (unsigned int i = 0; i < n; i++) {
		delete rind[i];
}
	delete[] rind;
	delete[] sind;
	delete[] indDiv;
	delete sclscm;
	delete slct;
	delete evaldata;
}

// Make a complete copy of the original population.  This is a deep copy of
// the population object - we clone everything in the genomes and copy all of
// the population's information.
void GAPopulation::copy(const GAPopulation &arg)
{
	unsigned int i;
	for (i = 0; i < n; i++) {
		delete rind[i];
}
	delete[] rind;
	delete[] sind;
	delete[] indDiv;
	delete sclscm;
	delete slct;
	delete evaldata;

	csz = arg.csz;
	N = arg.N;
	n = arg.n;
	rind = new GAGenome *[N];
	for (i = 0; i < n; i++) {
		rind[i] = arg.rind[i]->clone();
}
	sind = new GAGenome *[N];
	memcpy(sind, rind, N * sizeof(GAGenome *));

	if (arg.indDiv != nullptr)
	{
		indDiv = new float[N * N];
		memcpy(indDiv, arg.indDiv, (N * N * sizeof(float)));
	}
	else
	{
		indDiv = nullptr;
	}

	sclscm = arg.sclscm->clone();
	scaled = false;
	if (arg.scaled == true) {
		scale();
}

	slct = arg.slct->clone();
	slct->assign(*this);
	selectready = false;
	if (arg.selectready == true) {
		prepselect();
}

	if (arg.evaldata != nullptr) {
		evaldata = arg.evaldata->clone();
	} else {
		evaldata = nullptr;
}

	neval = 0; // don't copy the evaluation count!
	rawSum = arg.rawSum;
	rawAve = arg.rawAve;
	rawMax = arg.rawMax;
	rawMin = arg.rawMin;
	rawVar = arg.rawVar;
	rawDev = arg.rawDev;
	popDiv = arg.popDiv;

	fitSum = arg.fitSum;
	fitAve = arg.fitAve;
	fitMax = arg.fitMax;
	fitMin = arg.fitMin;
	fitVar = arg.fitVar;
	fitDev = arg.fitDev;

	sortorder = arg.sortorder;
	rsorted = arg.rsorted;
	ssorted = false; // we must sort at some later point
	statted = arg.statted;
	evaluated = arg.evaluated;
	divved = arg.divved;

	init = arg.init;
	eval = arg.eval;
	ud = arg.ud;
	ga = arg.ga;
}

// Resize the population.  If we shrink, we delete the extra genomes.  If
// we grow, we clone new ones (and we DO NOT initialize them!!!).  When we
// trash the genomes, we delete the worst of the population!  We do not
// free up the space used by the array of pointers, but we do free up the
// space used by the genomes.
//   We do a clone of the genome contents so that we don't have to initialize
// the new ones (what if the population has a custom initilizer?).  We randomly
// pick which ones to clone from the existing individuals.  If the population
// contains no genomes, then we post an error message (since there are no
// individuals from which to clone the new ones).
//   If the population was evaluated, then we evaluate the new genomes.  We
// do not sort nor restat the population, and we tag the statted and sorted
// flags to reflect the fact that they are no longer valid.
//   Resizing to a bigger size is the same as a batch 'add'
int GAPopulation::size(unsigned int popsize)
{
	if (popsize == n) {
		return n;
}
	if (n == 0 && popsize > 0)
	{
		GAErr(GA_LOC, "GAPopuluation", "size", GAError::NoIndividuals);
		return n;
	}

	if (popsize > n)
	{
		grow(popsize);
		for (unsigned int i = n; i < popsize; i++) {
			rind[i] = rind[GARandomInt(0, n - 1)]->clone(GAGenome::CloneMethod::CONTENTS);
}
		rsorted = false;
	}
	else
	{
		for (unsigned int i = popsize; i < n;
			 i++) { // trash the worst ones (if sorted)
			delete rind[i]; // may not be sorted!!!!
}
	}

	memcpy(sind, rind, N * sizeof(GAGenome *));
	ssorted = scaled = statted = divved = selectready = false;
	n = popsize;

	if (evaluated == true) {
		evaluate(true);
}

	return n;
}

// This is a private method for adjusting the size of the arrays used by the
// population object.  Unlike the size method, this method does not allocate
// more genomes (but it will delete genomes if the specified size is smaller
// than the current size).
//   This maintains the integrity of the diversity scores (but the new ones
// will not have been set yet).
//   We return the total amount allocated (not the amount used).
int GAPopulation::grow(unsigned int s)
{
	if (s <= N) {
		return N;
}

	int oldsize = N;
	while (N < s) {
		N += csz;
}

	GAGenome **tmp;

	tmp = rind;
	rind = new GAGenome *[N];
	memcpy(rind, tmp, oldsize * sizeof(GAGenome *));
	delete[] tmp;
	tmp = sind;
	sind = new GAGenome *[N];
	memcpy(sind, tmp, oldsize * sizeof(GAGenome *));
	delete[] tmp;

	if (indDiv != nullptr)
	{
		float *tmpd = indDiv;
		indDiv = new float[N * N];
		for (int i = 0; i < oldsize; i++) {
			memcpy(&(indDiv[i * N]), &(tmpd[i * oldsize]),
				   oldsize * sizeof(float));
}
		delete[] tmpd;
	}

	return N;
}

// Get rid of 'extra' memory that we have allocated.  We just trash the
// diversity matrix and flag it as being invalid.  Return the amount
// allocated (which is also the amount used).
int GAPopulation::compact()
{
	if (n == N) {
		return N;
}

	GAGenome **tmp;

	tmp = rind;
	rind = new GAGenome *[n];
	memcpy(rind, tmp, n * sizeof(GAGenome *));
	delete[] tmp;
	tmp = sind;
	sind = new GAGenome *[n];
	memcpy(sind, tmp, n * sizeof(GAGenome *));
	delete[] tmp;

	//  if(indDiv) {
	//    float *tmpd = indDiv;
	//    indDiv = new float [n*n];
	//    for(unsigned int i=0; i<n; i++)
	//      memcpy(&(indDiv[i*n]), &(tmpd[i*N]), n*sizeof(float));
	//    delete [] tmpd;
	//  }

	if (indDiv != nullptr)
	{
		delete[] indDiv;
		indDiv = nullptr;
	}

	return N = n;
}

GAPopulation::SortOrder GAPopulation::order(GAPopulation::SortOrder flag)
{
	if (sortorder == flag) {
		return flag;
}
	sortorder = flag;
	rsorted = ssorted = false;
	return flag;
}

// Sort using the quicksort method.  The sort order depends on whether a high
// number means 'best' or a low number means 'best'.  Individual 0 is always
// the 'best' individual, Individual n-1 is always the 'worst'.
//   We may sort either array of individuals - the array sorted by raw scores
// or the array sorted by scaled scores.
void GAPopulation::sort(bool flag, SortBasis basis) const
{
	auto *This = const_cast<GAPopulation *>(this);
	if (basis == RAW)
	{
		if (rsorted == false || flag == true)
		{
			if (sortorder == LOW_IS_BEST) {
				GAPopulation::QuickSortAscendingRaw(This->rind, 0, n - 1);
			} else {
				GAPopulation::QuickSortDescendingRaw(This->rind, 0, n - 1);
}
			This->selectready = false;
		}
		This->rsorted = true;
	}
	else if (basis == SCALED)
	{
		if (ssorted == false || flag == true)
		{
			if (sortorder == LOW_IS_BEST) {
				GAPopulation::QuickSortAscendingScaled(This->sind, 0, n - 1);
			} else {
				GAPopulation::QuickSortDescendingScaled(This->sind, 0, n - 1);
}
			This->selectready = false;
		}
		This->ssorted = true;
	}
}

// Evaluate each member of the population and store basic population statistics
// in the member variables.  It is OK to run this on a const object - it
// changes to physical state of the population, but not the logical state.
//   The partial sums are normalized to the range [0,1] so that they can be
// used whether the population is sorted as low-is-best or high-is-best.
// Individual 0 is always the best individual, and the partial sums are
// calculated so that the worst individual has the smallest partial sum.  All
// of the partial sums add to 1.0.
void GAPopulation::statistics(bool flag) const
{
	if (statted == true && flag != true) {
		return;
}
	auto *This = const_cast<GAPopulation *>(this);

	if (n > 0)
	{
		float tmpsum;
		This->rawMin = This->rawMax = tmpsum = rind[0]->score();

		unsigned int i;
		for (i = 1; i < n; i++)
		{
			float scr = rind[i]->score();
			tmpsum += scr;
			This->rawMax = GAMax(rawMax, scr);
			This->rawMin = GAMin(rawMin, scr);
		}
		float tmpave = tmpsum / n;
		This->rawAve = tmpave;
		This->rawSum = tmpsum; // if scores are huge we'll lose data here

		float tmpvar = 0.0;
		if (n > 1)
		{
			for (i = 0; i < n; i++)
			{
				float s = rind[i]->score() - This->rawAve;
				s *= s;
				tmpvar += s;
			}
			tmpvar /= (n - 1);
		}
		This->rawDev = sqrt(tmpvar);
		This->rawVar = tmpvar; // could lose data if huge variance
	}
	else
	{
		This->rawMin = This->rawMax = This->rawSum = 0.0;
		This->rawDev = This->rawVar = 0.0;
	}

	This->statted = true;
}

// Do the scaling on the population.  Like the statistics and diversity, this
// method does not change the contents of the population, but it does change
// the values of the status members of the object.  So we allow it to work on
// a const population.
void GAPopulation::scale(bool flag) const
{
	if (scaled == true && flag != true) {
		return;
}
	auto *This = const_cast<GAPopulation *>(this);

	if (n > 0)
	{
		sclscm->evaluate(*This);

		float tmpsum;
		This->fitMin = This->fitMax = tmpsum = sind[0]->fitness();

		unsigned int i;
		for (i = 1; i < n; i++)
		{
			tmpsum += sind[i]->fitness();
			This->fitMax = GAMax(fitMax, sind[i]->fitness());
			This->fitMin = GAMin(fitMin, sind[i]->fitness());
		}
		float tmpave = tmpsum / n;
		This->fitAve = tmpave;
		This->fitSum = tmpsum; // if scores are huge we'll lose data here

		float tmpvar = 0.0;
		if (n > 1)
		{
			for (i = 0; i < n; i++)
			{
				float s = sind[i]->fitness() - This->fitAve;
				s *= s;
				tmpvar += s;
			}
			tmpvar /= (n - 1);
		}
		This->fitDev = sqrt(tmpvar);
		This->fitVar = tmpvar; // could lose data if huge variance
	}
	else
	{
		This->fitMin = This->fitMax = This->fitSum = 0.0;
		This->fitVar = This->fitDev = 0.0;
	}

	This->scaled = true;
	This->ssorted = false;
}

// Calculate the population's diversity score.  The matrix is triangular and
// we don't have to calculate the diagonals.  This assumes that div(i,j) is
// the same as div(j,i) (for our purposes this will always be true, but it is
// possible for someone to override some of the individuals in the population
// and not others).
//   For now we keep twice as many diversity numbers as we need.  We need only
// n*(n-1)/2, but I can't seem to figure out an efficient way to map i,j to the
// reduced n*(n-1)/2 set (remember that the diagonals are always 0.0).
//   The diversity of the entire population is just the average of all the
// individual diversities.  So if every individual is completely different from
// all of the others, the population diversity is > 0.  If they are all the
// same, the diversity is 0.0.  We don't count the diagonals for the population
// diversity measure.  0 means minimal diversity means all the same.
void GAPopulation::diversity(bool flag) const
{
	if (divved == true && flag != true) {
		return;
}
	auto *This = const_cast<GAPopulation *>(this);

	if (n > 1)
	{
		if (This->indDiv == nullptr) {
			This->indDiv = new float[N * N];
}

		This->popDiv = 0.0;
		for (unsigned int i = 0; i < n; i++)
		{
			This->indDiv[i * n + i] = 0.0;
			for (unsigned int j = i + 1; j < n; j++)
			{
				This->indDiv[j * n + i] = This->indDiv[i * n + j] =
					individual(i).compare(individual(j));
				This->popDiv += indDiv[i * n + j];
			}
		}
		This->popDiv /= n * (n - 1) / 2;
	}
	else
	{
		This->popDiv = 0.0;
	}

	This->divved = true;
}

void GAPopulation::prepselect(bool flag) const
{
	if (selectready == true && flag != true) {
		return;
}
	auto *This = const_cast<GAPopulation *>(this);
	This->slct->update();
	This->selectready = true;
}

// Return a reference to the scaling object.
GAScalingScheme &GAPopulation::scaling(const GAScalingScheme &s)
{
	delete sclscm;
	sclscm = s.clone();
	scaled = false;
	return *sclscm;
}

// Return a reference to the selection object.
GASelectionScheme &GAPopulation::selector(const GASelectionScheme &s)
{
	delete slct;
	slct = s.clone();
	slct->assign(*this);
	selectready = false;
	return *slct;
}

// Replace the specified genome with the one that is passed to us then
// return the one that got replaced.  Use the replacement flags to determine
// which genome will be replaced.  If we get a genome as the second
// argument, then replace that one.  If we get a NULL genome, then we
// return a NULL and don't do anything.
//   If the population is sorted, then we maintain the sort by doing a smart
// replacement.
//   If the population is not sorted, then we just do the replacement without
// worrying about the sort.  Replace best and worst both require that we know
// which chromsomes are which, so we do a sort before we do the replacement,
// then we do a smart replacement.
//   In both cases we flag the stats as out-of-date, but we do not update the
// stats.  Let that happen when it needs to happen.
//   If which is < 0 then it is a flag that tells us to do a certain kind of
// replacement.  Anything non-negative is assumed to be an index to a
// genome in the population.
//   This does not affect the state of the evaluated member - it assumes that
// the individual genome has a valid number for its score.
GAGenome *GAPopulation::replace(GAGenome *repl, int which, SortBasis basis)
{
	int i = -1;
	GAGenome *orig = nullptr;
	if (repl == nullptr) {
		return orig;
}

	switch (which)
	{
	case BEST:
		sort(false, basis);
		i = 0;
		break;

	case WORST:
		sort(false, basis);
		i = n - 1;
		break;

	case RANDOM:
		i = GARandomInt(0, n - 1);
		break;

	default:
		if (0 <= which && which < static_cast<int>(n)) {
			i = which;
}
		break;
	}

	if (i >= 0)
	{
		// We could insert this properly if the population is sorted, but that
		// would require us to evaluate the genome, and we don't want to do that
		// 'cause that will screw up any parallel implementations.  So we just
		// stick it in the population and let the sort take care of it at a
		// later time as needed.
		if (basis == RAW)
		{
			orig = rind[i]; // keep the original to return at the end
			rind[i] = repl;
			memcpy(sind, rind, N * sizeof(GAGenome *));
		}
		else
		{
			orig = sind[i]; // keep the original to return at the end
			sind[i] = repl;
			memcpy(rind, sind, N * sizeof(GAGenome *));
		}
		rsorted = ssorted = false; // must sort again
		// flag for recalculate stats
		statted = false;
		// Must flag for a new evaluation.
		evaluated = false;
		// No way to do incremental update of scaling info since we don't know
		// what the scaling object will do.
		scaled = false;
		// *** should do an incremental update of the diversity here so we don't
		// recalculate all of the diversities when only one is updated
		divved = false;
		// selector needs update
		selectready = false;

		// make sure the genome has the correct genetic algorithm pointer
		if (ga != nullptr) {
			repl->geneticAlgorithm(*ga);
}
	}

	return orig;
}

// Replace the genome o in the population with the genome r.  Return a
// pointer to the original genome, o.  This assumes that o exists in the
// population.   If it does not, we return a NULL.  If the genomes are the
// same, do nothing and return a pointer to the genome.
GAGenome *GAPopulation::replace(GAGenome *r, GAGenome *o)
{
	GAGenome *orig = nullptr;
	if (r == nullptr || o == nullptr) {
		return orig;
}
	if (r == o) {
		return r;
}
	unsigned int i;
	for (i = 0; i < n && rind[i] != o; i++) {
		;
}
	if (i < n) {
		orig = replace(r, i, RAW);
}
	return orig;
}

//   Remove the xth genome from the population.  If index is out of bounds, we
// return NULL.  Otherwise we return a pointer to the genome that was
// removed.  The population is now no longer responsible for freeing the
// memory used by that genome.
//   We don't touch the sorted flag for the array we modify - a remove will not
// affect the sort order.
GAGenome *GAPopulation::remove(int i, SortBasis basis)
{
	GAGenome *removed = nullptr;
	if (i == BEST)
	{
		sort(false, basis);
		i = 0;
	}
	else if (i == WORST)
	{
		sort(false, basis);
		i = n - 1;
	}
	else if (i == RANDOM) {
		i = GARandomInt(0, n - 1);
	} else if (i < 0 || i >= static_cast<int>(n)) {
		return removed;
}

	if (basis == RAW)
	{
		removed = rind[i];
		memmove(&(rind[i]), &(rind[i + 1]), (n - i - 1) * sizeof(GAGenome *));
		memcpy(sind, rind, N * sizeof(GAGenome *));
		ssorted = false;
	}
	else if (basis == SCALED)
	{
		removed = sind[i];
		memmove(&(sind[i]), &(sind[i + 1]), (n - i - 1) * sizeof(GAGenome *));
		memcpy(rind, sind, N * sizeof(GAGenome *));
		rsorted = false;
	}
	else {
		return removed;
}

	n--;
	evaluated = false;

	// *** should be smart about these and do incremental update?
	scaled = statted = divved = selectready = false;

	return removed;
}

//   Remove the specified genome from the population.  If the genome is
// not in the population, we return NULL.  We do a linear search here (yuk for
// large pops, but little else we can do).  The memory used by the genome is
// now the responsibility of the caller.
GAGenome *GAPopulation::remove(GAGenome *r)
{
	GAGenome *removed = nullptr;
	if (r == nullptr) {
		return removed;
}
	unsigned int i;
	for (i = 0; i < n && rind[i] != r; i++) {
		;
}
	if (i < n) {
		removed = remove(i, RAW);
}
	return removed;
}

// Add the specified individual to the population.  We don't update the stats
// or sort - let those get updated next time they are needed.
//   Notice that it is possible to add individuals to the population that are
// not the same type as the other genomes in the population.  Eventually we
// probably won't allow this (or at least we'll have to fix things so that the
// behaviour is completely defined).
//   If you invoke the add with a genome reference, the population will make
// a clone of the genome then it owns it from then on.  If you invoke add with
// a genome pointer, then the population does not allocate any memory - it uses
// the memory pointed to by the argument.  So don't trash the genome without
// first letting the population know about the change.
GAGenome *GAPopulation::add(const GAGenome &g)
{
	return GAPopulation::add(g.clone());
}

// This one does *not* allocate space for the genome - it uses the one that
// was passed to us.  So the caller should not free it up or leave it dangling!
// We own it from now on (unless remove is called on it), and the population
// will destroy it when the population destructor is invoked.
GAGenome *GAPopulation::add(GAGenome *c)
{
	if (c == nullptr) {
		return c;
}
	grow(n + 1);
	rind[n] = sind[n] = c;
	if (ga != nullptr) {
		rind[n]->geneticAlgorithm(*ga);
}
	n++;

	rsorted = ssorted = false; // may or may not be true, but must be sure
	evaluated = scaled = statted = divved = selectready = false;

	return c;
}

GAGeneticAlgorithm *GAPopulation::geneticAlgorithm(GAGeneticAlgorithm &g)
{
	for (unsigned int i = 0; i < n; i++) {
		rind[i]->geneticAlgorithm(g);
}
	return (ga = &g);
}

void GAPopulation::write(std::ostream &os, SortBasis basis) const
{
	for (unsigned int i = 0; i < n; i++)
	{
		if (basis == RAW) {
			os << *rind[i] << "\n";
		} else {
			os << *sind[i] << "\n";
}
	}
	os << "\n";
}

void GAPopulation::QuickSortAscendingRaw(GAGenome **c, int l, int r)
{

	GAGenome *t;
	if (r > l)
	{
		float v = c[r]->score();
		int i = l - 1;
		int j = r;
		for (;;)
		{
			while (c[++i]->score() < v && i <= r) {
				;
}
			while (c[--j]->score() > v && j > 0) {
				;
}
			if (i >= j) {
				break;
}
			t = c[i];
			c[i] = c[j];
			c[j] = t;
		}
		t = c[i];
		c[i] = c[r];
		c[r] = t;
		GAPopulation::QuickSortAscendingRaw(c, l, i - 1);
		GAPopulation::QuickSortAscendingRaw(c, i + 1, r);
	}
}
void GAPopulation::QuickSortDescendingRaw(GAGenome **c, int l, int r)
{
	GAGenome *t;
	if (r > l)
	{
		float v = c[r]->score();
		int i = l - 1;
		int j = r;
		for (;;)
		{
			while (c[++i]->score() > v && i <= r) {
				;
}
			while (c[--j]->score() < v && j > 0) {
				;
}
			if (i >= j) {
				break;
}
			t = c[i];
			c[i] = c[j];
			c[j] = t;
		}
		t = c[i];
		c[i] = c[r];
		c[r] = t;
		GAPopulation::QuickSortDescendingRaw(c, l, i - 1);
		GAPopulation::QuickSortDescendingRaw(c, i + 1, r);
	}
}

void GAPopulation::QuickSortAscendingScaled(GAGenome **c, int l, int r)
{
	GAGenome *t;
	if (r > l)
	{
		float v = c[r]->fitness();
		int i = l - 1;
		int j = r;
		for (;;)
		{
			while (c[++i]->fitness() < v && i <= r) {
				;
}
			while (c[--j]->fitness() > v && j > 0) {
				;
}
			if (i >= j) {
				break;
}
			t = c[i];
			c[i] = c[j];
			c[j] = t;
		}
		t = c[i];
		c[i] = c[r];
		c[r] = t;
		GAPopulation::QuickSortAscendingScaled(c, l, i - 1);
		GAPopulation::QuickSortAscendingScaled(c, i + 1, r);
	}
}
void GAPopulation::QuickSortDescendingScaled(GAGenome **c, int l, int r)
{
	GAGenome *t;
	if (r > l)
	{
		float v = c[r]->fitness();
		int i = l - 1;
		int j = r;
		for (;;)
		{
			while (c[++i]->fitness() > v && i <= r) {
				;
}
			while (c[--j]->fitness() < v && j > 0) {
				;
}
			if (i >= j) {
				break;
}
			t = c[i];
			c[i] = c[j];
			c[j] = t;
		}
		t = c[i];
		c[i] = c[r];
		c[r] = t;
		GAPopulation::QuickSortDescendingScaled(c, l, i - 1);
		GAPopulation::QuickSortDescendingScaled(c, i + 1, r);
	}
}
