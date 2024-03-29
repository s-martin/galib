// $Header$
/* ----------------------------------------------------------------------------
  array3.h
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This header defines the interface for the 3D array genome.  See comments in
1D array file.
---------------------------------------------------------------------------- */
#ifndef _ga_array3_h_
#define _ga_array3_h_

#include <GAAllele.h>
#include <GAArray.h>
#include <GAGenome.h>

#include <vector>

/* ----------------------------------------------------------------------------
3DArrayGenome
---------------------------------------------------------------------------- */
template <class T> class GA3DArrayGenome : public GAArray<T>, public GAGenome
{
  public:
	GADeclareIdentity();

	static int SwapMutator(GAGenome &, float);
	static float ElementComparator(const GAGenome &, const GAGenome &);
	static int UniformCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								GAGenome *);
	static int EvenOddCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								GAGenome *);
	static int OnePointCrossover(const GAGenome &, const GAGenome &, GAGenome *,
								 GAGenome *);

  public:
	GA3DArrayGenome(unsigned int x, unsigned int y, unsigned int z,
					GAGenome::Evaluator f = (GAGenome::Evaluator)nullptr,
					void *u = nullptr);
	GA3DArrayGenome(const GA3DArrayGenome &orig);
	GA3DArrayGenome &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	GA3DArrayGenome &operator=(const T array[])
	{
		for (unsigned int i = 0; i < nx; i++)
			for (unsigned int j = 0; j < ny; j++)
				for (unsigned int k = 0; k < nz; k++)
					gene(i, j, k, *(array + k * ny * nx + j * nx + i));
		return *this;
	}
	~GA3DArrayGenome() override;
	GAGenome *clone(GAGenome::CloneMethod flag = CloneMethod::CONTENTS) const override;
	void copy(const GAGenome &chrom) override;

	int read(std::istream &) override;
	int write(std::ostream &) const override;

	bool equal(const GAGenome &c) const override;

	const T &gene(unsigned int x, unsigned int y, unsigned int z) const
	{
		return this->a[z * ny * nx + y * nx + x];
	}
	T &gene(unsigned int x, unsigned int y, unsigned int z, const T &value)
	{
		if (this->a[z * ny * nx + y * nx + x] != value)
		{
			_evaluated = false;
			this->a[z * ny * nx + y * nx + x] = value;
		}
		return this->a[z * ny * nx + y * nx + x];
	}
	int width() const { return nx; }
	int width(int w)
	{
		resize(w, ny, nz);
		return nx;
	}
	int height() const { return ny; }
	int height(int h)
	{
		resize(nx, h, nz);
		return ny;
	}
	int depth() const { return nz; }
	int depth(int d)
	{
		resize(nx, ny, d);
		return nz;
	}
	virtual int resize(int x, int y, int z);
	int resizeBehaviour(Dimension which) const;
	int resizeBehaviour(Dimension which, unsigned int lower,
						unsigned int upper);
	int resizeBehaviour(unsigned int lowerX, unsigned int upperX,
						unsigned int lowerY, unsigned int upperY,
						unsigned int lowerZ, unsigned int upperZ)
	{
		return (resizeBehaviour(Dimension::WIDTH, lowerX, upperX) *
				resizeBehaviour(Dimension::HEIGHT, lowerY, upperY) *
				resizeBehaviour(Dimension::DEPTH, lowerZ, upperZ));
	}
	void copy(const GA3DArrayGenome &, unsigned int, unsigned int, unsigned int,
			  unsigned int, unsigned int, unsigned int, unsigned int,
			  unsigned int, unsigned int);
	void swap(unsigned int a, unsigned int b, unsigned int c, unsigned int d,
			  unsigned int e, unsigned int f)
	{
		GAArray<T>::swap(c * ny * nx + b * nx + a, f * ny * nx + e * nx + d);
		_evaluated = false;
	}

  protected:
	unsigned int nx, ny, nz, minX, minY, minZ, maxX, maxY, maxZ;
};

/* ----------------------------------------------------------------------------
3DArrayAlleleGenome
---------------------------------------------------------------------------- */
template <class T> class GA3DArrayAlleleGenome : public GA3DArrayGenome<T>
{
  public:
	GADeclareIdentity();

	static void UniformInitializer(GAGenome &);
	static int FlipMutator(GAGenome &, float);

  public:
	GA3DArrayAlleleGenome(unsigned int x, unsigned int y, unsigned int z,
						  const GAAlleleSet<T> &a,
						  GAGenome::Evaluator f = nullptr,
						  void *u = nullptr);
	GA3DArrayAlleleGenome(unsigned int x, unsigned int y, unsigned int z,
						  const GAAlleleSetArray<T> &a,
						  GAGenome::Evaluator f = nullptr,
						  void *u = nullptr);
	GA3DArrayAlleleGenome(const GA3DArrayAlleleGenome<T> &orig);
	GA3DArrayAlleleGenome<T> &operator=(const GAGenome &orig)
	{
		copy(orig);
		return *this;
	}
	GA3DArrayAlleleGenome<T> &operator=(const T array[])
	{
		GA3DArrayGenome<T>::operator=(array);
		return *this;
	}
	virtual ~GA3DArrayAlleleGenome();
	GAGenome *
	clone(GAGenome::CloneMethod flag = GAGenome::CloneMethod::CONTENTS) const override;
	void copy(const GAGenome &) override;

	int read(std::istream &is) override;
	int write(std::ostream &os) const override;

	int equal(const GAGenome &c) const override;
	int resize(int x, int y, int z) override;

	const GAAlleleSet<T> &alleleset(unsigned int i = 0) const
	{
		return aset.at(i % aset.size());
	}

  protected:
	std::vector<GAAlleleSet<T>> aset;
};

#endif
