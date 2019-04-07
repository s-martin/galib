// $Header$
/* ----------------------------------------------------------------------------
  array2.h
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This header defines the interface for the 2D array genome.  See comments in
1D array file.
---------------------------------------------------------------------------- */
#ifndef _ga_array2_h_
#define _ga_array2_h_

#include <GAAllele.h>
#include <GAArray.h>
#include <GAGenome.h>

/* ----------------------------------------------------------------------------
2DArrayGenome
---------------------------------------------------------------------------- */
template <class T> class GA2DArrayGenome : public GAArray<T>, public GAGenome
{
  public:
	GADeclareIdentity();

	static int SwapMutator(const GAGenome &, float);
	static float ElementComparator(const GAGenome &, const GAGenome &);
	static int UniformCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								GAGenome *);
	static int EvenOddCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								GAGenome *);
	static int OnePointCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								 GAGenome *);

  public:
	GA2DArrayGenome(unsigned int x, unsigned int y,
					GAGenome::Evaluator f = (GAGenome::Evaluator)0,
					void *u = nullptr);
	GA2DArrayGenome(const GA2DArrayGenome<T> &orig);
	GA2DArrayGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	GA2DArrayGenome<T> &operator=(const T array[])
	{
		for (unsigned int i = 0; i < nx; i++)
			for (unsigned int j = 0; j < ny; j++)
				gene(i, j, *(array + j * nx + i));
		return *this;
	}
	~GA2DArrayGenome() override;
	GAGenome *clone(GAGenome::CloneMethod flag = CONTENTS) const override;
	void copy(const GAGenome &chrom) override;

	int read(std::istream &is) override;
	int write(std::ostream &os) const override;

	bool equal(const GAGenome &c) const override;

	const T &gene(unsigned int x, unsigned int y) const
	{
		return this->a[y * nx + x];
	}
	T &gene(unsigned int x, unsigned int y, const T &value)
	{
		if (this->a[y * nx + x] != value)
		{
			this->a[y * nx + x] = value;
			_evaluated = false;
		}
		return this->a[y * nx + x];
	}
	int width() const { return nx; }
	int width(int w)
	{
		resize(w, ny);
		return nx;
	}
	int height() const { return ny; }
	int height(int h)
	{
		resize(nx, h);
		return ny;
	}
	virtual int resize(int w, int h);
	int resizeBehaviour(Dimension which) const;
	int resizeBehaviour(Dimension which, unsigned int lower,
						unsigned int upper);
	int resizeBehaviour(unsigned int lowerX, unsigned int upperX,
						unsigned int lowerY, unsigned int upperY)
	{
		return (resizeBehaviour(Dimension::WIDTH, lowerX, upperX) *
				resizeBehaviour(Dimension::HEIGHT, lowerY, upperY));
	}
	void copy(const GA2DArrayGenome &, unsigned int, unsigned int, unsigned int,
			  unsigned int, unsigned int, unsigned int);
	void swap(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
	{
		GAArray<T>::swap(b * nx + a, d * nx + c);
		_evaluated = false;
	}

  protected:
	GAAlleleSet<T> *as; // the allele set
	unsigned int nx, ny, minX, minY, maxX, maxY;
};

/* ----------------------------------------------------------------------------
2DArrayAlleleGenome
---------------------------------------------------------------------------- */
template <class T> class GA2DArrayAlleleGenome : public GA2DArrayGenome<T>
{
  public:
	GADeclareIdentity();

	static void UniformInitializer(const GAGenome &);
	static int FlipMutator(const GAGenome &, float);

  public:
	GA2DArrayAlleleGenome(unsigned int x, unsigned int y,
						  const GAAlleleSet<T> &a,
						  GAGenome::Evaluator f = (GAGenome::Evaluator)0,
						  void *u = nullptr);
	GA2DArrayAlleleGenome(unsigned int x, unsigned int y,
						  const GAAlleleSetArray<T> &a,
						  GAGenome::Evaluator f = (GAGenome::Evaluator)0,
						  void *u = nullptr);
	GA2DArrayAlleleGenome(const GA2DArrayAlleleGenome<T> &orig);
	GA2DArrayAlleleGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	GA2DArrayAlleleGenome<T> &operator=(const T array[])
	{
		GA2DArrayGenome<T>::operator=(array);
		return *this;
	}
	virtual ~GA2DArrayAlleleGenome();
	GAGenome *
	clone(GAGenome::CloneMethod flag = GAGenome::CONTENTS) const override;
	void copy(const GAGenome &) override;

	int read(std::istream &is) override;
	int write(std::ostream &os) const override;

	int equal(const GAGenome &c) const override;
	int resize(int x, int y) override;

	const GAAlleleSet<T> &alleleset(unsigned int i = 0) const
	{
		return aset[i % naset];
	}

  protected:
	int naset;
	GAAlleleSet<T> *aset;
};

#endif
