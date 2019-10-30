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
#include <GAMask.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <garandom.h>

/* ----------------------------------------------------------------------------
2DArrayGenome
---------------------------------------------------------------------------- */
template <class T> class GA2DArrayGenome : public GAArray<T>, public GAGenome
{
  public:
	GADefineIdentity("GA2DArrayGenome", GAID::ArrayGenome2D);

	static int SwapMutator(GAGenome &c, float pmut)
	{
		GA2DArrayGenome<T> &child = DYN_CAST(GA2DArrayGenome<T> &, c);

		if (pmut <= 0.0)
			return (0);

		float nMut = pmut * STA_CAST(float, child.size());
		int size = child.size() - 1;
		if (nMut < 1.0)
		{ // we have to do a flip test on each bit
			nMut = 0;
			for (int i = size; i >= 0; i--)
			{
				if (GAFlipCoin(pmut))
				{
					child.GAArray<T>::swap(i, GARandomInt(0, size));
					nMut++;
				}
			}
		}
		else
		{ // only flip the number of bits we need to flip
			for (int n = 0; n < nMut; n++)
				child.GAArray<T>::swap(GARandomInt(0, size),
									   GARandomInt(0, size));
		}
		return (STA_CAST(int, nMut));
	}

	static float ElementComparator(const GAGenome &a, const GAGenome &b)
	{
		const GA2DArrayGenome<T> &sis = DYN_CAST(const GA2DArrayGenome<T> &, a);
		const GA2DArrayGenome<T> &bro = DYN_CAST(const GA2DArrayGenome<T> &, b);
		if (sis.size() != bro.size())
			return -1;
		if (sis.size() == 0)
			return 0;
		float count = 0.0;
		for (int i = sis.width() - 1; i >= 0; i--)
			for (int j = sis.height() - 1; j >= 0; j--)
				count += ((sis.gene(i, j) == bro.gene(i, j)) ? 0 : 1);
		return count / sis.size();
	}

	static int UniformCrossover(const GAGenome &p1, const GAGenome &p2,
								GAGenome *c1, GAGenome *c2)
	{
		const GA2DArrayGenome<T> &mom =
			DYN_CAST(const GA2DArrayGenome<T> &, p1);
		const GA2DArrayGenome<T> &dad =
			DYN_CAST(const GA2DArrayGenome<T> &, p2);

		int nc = 0;
		int i, j;

		if (c1 && c2)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);
			GA2DArrayGenome<T> &bro = DYN_CAST(GA2DArrayGenome<T> &, *c2);

			if (sis.width() == bro.width() && sis.height() == bro.height() &&
				mom.width() == dad.width() && mom.height() == dad.height() &&
				sis.width() == mom.width() && sis.height() == mom.height())
			{
				for (i = sis.width() - 1; i >= 0; i--)
				{
					for (j = sis.height() - 1; j >= 0; j--)
					{
						if (GARandomBit())
						{
							sis.gene(i, j, mom.gene(i, j));
							bro.gene(i, j, dad.gene(i, j));
						}
						else
						{
							sis.gene(i, j, dad.gene(i, j));
							bro.gene(i, j, mom.gene(i, j));
						}
					}
				}
			}
			else
			{
				GAMask mask;
				int startx, starty;
				int maxx =
					(sis.width() > bro.width()) ? sis.width() : bro.width();
				int minx =
					(mom.width() < dad.width()) ? mom.width() : dad.width();
				int maxy =
					(sis.height() > bro.height()) ? sis.height() : bro.height();
				int miny =
					(mom.height() < dad.height()) ? mom.height() : dad.height();
				mask.size(maxx * maxy);
				for (i = 0; i < maxx; i++)
					for (j = 0; j < maxy; j++)
						mask[i * maxy + j] = GARandomBit();
				startx = (sis.width() < minx) ? sis.width() : minx;
				starty = (sis.height() < miny) ? sis.height() : miny;
				for (i = startx - 1; i >= 0; i--)
					for (j = starty - 1; j >= 0; j--)
						sis.gene(i, j,
								 (mask[i * starty + j] ? mom.gene(i, j)
													   : dad.gene(i, j)));
				startx = (bro.width() < minx) ? bro.width() : minx;
				starty = (bro.height() < miny) ? bro.height() : miny;
				for (i = startx - 1; i >= 0; i--)
					for (j = starty - 1; j >= 0; j--)
						bro.gene(i, j,
								 (mask[i * starty + j] ? dad.gene(i, j)
													   : mom.gene(i, j)));
			}

			nc = 2;
		}
		else if (c1)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);

			if (mom.width() == dad.width() && mom.height() == dad.height() &&
				sis.width() == mom.width() && sis.height() == mom.height())
			{
				for (i = sis.width() - 1; i >= 0; i--)
					for (j = sis.height() - 1; j >= 0; j--)
						sis.gene(
							i, j,
							(GARandomBit() ? mom.gene(i, j) : dad.gene(i, j)));
			}
			else
			{
				int minx =
					(mom.width() < dad.width()) ? mom.width() : dad.width();
				int miny =
					(mom.height() < dad.height()) ? mom.height() : dad.height();
				minx = (sis.width() < minx) ? sis.width() : minx;
				miny = (sis.height() < miny) ? sis.height() : miny;
				for (i = minx - 1; i >= 0; i--)
					for (j = miny - 1; j >= 0; j--)
						sis.gene(
							i, j,
							(GARandomBit() ? mom.gene(i, j) : dad.gene(i, j)));
			}

			nc = 1;
		}

		return nc;
	}

	// This crossover does clipping (no padding) for resizables.  Notice that
	// this means that any resizable children of two parents will have identical
	// dimensions no matter what.
	static int OnePointCrossover(const GAGenome &p1, const GAGenome &p2,
								 GAGenome *c1, GAGenome *c2)
	{
		const GA2DArrayGenome<T> &mom =
			DYN_CAST(const GA2DArrayGenome<T> &, p1);
		const GA2DArrayGenome<T> &dad =
			DYN_CAST(const GA2DArrayGenome<T> &, p2);

		int nc = 0;
		unsigned int momsitex, momlenx, momsitey, momleny;
		unsigned int dadsitex, dadlenx, dadsitey, dadleny;
		unsigned int sitex, lenx, sitey, leny;

		if (c1 && c2)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);
			GA2DArrayGenome<T> &bro = DYN_CAST(GA2DArrayGenome<T> &, *c2);

			if (sis.resizeBehaviour(Dimension::WIDTH) == GAGenome::FIXED_SIZE &&
				bro.resizeBehaviour(Dimension::WIDTH) == GAGenome::FIXED_SIZE)
			{
				if (mom.width() != dad.width() || sis.width() != bro.width() ||
					sis.width() != mom.width())
				{
					GAErr(GA_LOC, mom.className(), "one-point cross",
						  GAError::SameLengthReqd);
					return nc;
				}
				sitex = momsitex = dadsitex = GARandomInt(0, mom.width());
				lenx = momlenx = dadlenx = mom.width() - momsitex;
			}
			else if (sis.resizeBehaviour(Dimension::WIDTH) ==
						 GAGenome::FIXED_SIZE ||
					 bro.resizeBehaviour(Dimension::WIDTH) ==
						 GAGenome::FIXED_SIZE)
			{
				GAErr(GA_LOC, mom.className(), "one-point cross",
					  GAError::SameBehavReqd);
				return nc;
			}
			else
			{
				momsitex = GARandomInt(0, mom.width());
				dadsitex = GARandomInt(0, dad.width());
				momlenx = mom.width() - momsitex;
				dadlenx = dad.width() - dadsitex;
				sitex = GAMin(momsitex, dadsitex);
				lenx = GAMin(momlenx, dadlenx);
			}

			if (sis.resizeBehaviour(Dimension::HEIGHT) ==
					GAGenome::FIXED_SIZE &&
				bro.resizeBehaviour(Dimension::HEIGHT) == GAGenome::FIXED_SIZE)
			{
				if (mom.height() != dad.height() ||
					sis.height() != bro.height() ||
					sis.height() != mom.height())
				{
					GAErr(GA_LOC, mom.className(), "one-point cross",
						  GAError::SameLengthReqd);
					return nc;
				}
				sitey = momsitey = dadsitey = GARandomInt(0, mom.height());
				leny = momleny = dadleny = mom.height() - momsitey;
			}
			else if (sis.resizeBehaviour(Dimension::HEIGHT) ==
						 GAGenome::FIXED_SIZE ||
					 bro.resizeBehaviour(Dimension::HEIGHT) ==
						 GAGenome::FIXED_SIZE)
			{
				GAErr(GA_LOC, mom.className(), "one-point cross",
					  GAError::SameBehavReqd);
				return nc;
			}
			else
			{
				momsitey = GARandomInt(0, mom.height());
				dadsitey = GARandomInt(0, dad.height());
				momleny = mom.height() - momsitey;
				dadleny = dad.height() - dadsitey;
				sitey = GAMin(momsitey, dadsitey);
				leny = GAMin(momleny, dadleny);
			}

			sis.resize(sitex + lenx, sitey + leny);
			bro.resize(sitex + lenx, sitey + leny);

			sis.copy(mom, 0, 0, momsitex - sitex, momsitey - sitey, sitex,
					 sitey);
			sis.copy(dad, sitex, 0, dadsitex, dadsitey - sitey, lenx, sitey);
			sis.copy(dad, 0, sitey, dadsitex - sitex, dadsitey, sitex, leny);
			sis.copy(mom, sitex, sitey, momsitex, momsitey, lenx, leny);

			bro.copy(dad, 0, 0, dadsitex - sitex, dadsitey - sitey, sitex,
					 sitey);
			bro.copy(mom, sitex, 0, momsitex, momsitey - sitey, lenx, sitey);
			bro.copy(mom, 0, sitey, momsitex - sitex, momsitey, sitex, leny);
			bro.copy(dad, sitex, sitey, dadsitex, dadsitey, lenx, leny);

			nc = 2;
		}
		else if (c1)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);

			if (sis.resizeBehaviour(Dimension::WIDTH) == GAGenome::FIXED_SIZE)
			{
				if (mom.width() != dad.width() || sis.width() != mom.width())
				{
					GAErr(GA_LOC, mom.className(), "one-point cross",
						  GAError::SameLengthReqd);
					return nc;
				}
				sitex = momsitex = dadsitex = GARandomInt(0, mom.width());
				lenx = momlenx = dadlenx = mom.width() - momsitex;
			}
			else
			{
				momsitex = GARandomInt(0, mom.width());
				dadsitex = GARandomInt(0, dad.width());
				momlenx = mom.width() - momsitex;
				dadlenx = dad.width() - dadsitex;
				sitex = GAMin(momsitex, dadsitex);
				lenx = GAMin(momlenx, dadlenx);
			}

			if (sis.resizeBehaviour(Dimension::HEIGHT) == GAGenome::FIXED_SIZE)
			{
				if (mom.height() != dad.height() ||
					sis.height() != mom.height())
				{
					GAErr(GA_LOC, mom.className(), "one-point cross",
						  GAError::SameLengthReqd);
					return nc;
				}
				sitey = momsitey = dadsitey = GARandomInt(0, mom.height());
				leny = momleny = dadleny = mom.height() - momsitey;
			}
			else
			{
				momsitey = GARandomInt(0, mom.height());
				dadsitey = GARandomInt(0, dad.height());
				momleny = mom.height() - momsitey;
				dadleny = dad.height() - dadsitey;
				sitey = GAMin(momsitey, dadsitey);
				leny = GAMin(momleny, dadleny);
			}

			sis.resize(sitex + lenx, sitey + leny);

			if (GARandomBit())
			{
				sis.copy(mom, 0, 0, momsitex - sitex, momsitey - sitey, sitex,
						 sitey);
				sis.copy(dad, sitex, 0, dadsitex, dadsitey - sitey, lenx,
						 sitey);
				sis.copy(dad, 0, sitey, dadsitex - sitex, dadsitey, sitex,
						 leny);
				sis.copy(mom, sitex, sitey, momsitex, momsitey, lenx, leny);
			}
			else
			{
				sis.copy(dad, 0, 0, dadsitex - sitex, dadsitey - sitey, sitex,
						 sitey);
				sis.copy(mom, sitex, 0, momsitex, momsitey - sitey, lenx,
						 sitey);
				sis.copy(mom, 0, sitey, momsitex - sitex, momsitey, sitex,
						 leny);
				sis.copy(dad, sitex, sitey, dadsitex, dadsitey, lenx, leny);
			}

			nc = 1;
		}

		return nc;
	}

	static int EvenOddCrossover(const GAGenome &p1, const GAGenome &p2,
								GAGenome *c1, GAGenome *c2)
	{
		const GA2DArrayGenome<T> &mom =
			DYN_CAST(const GA2DArrayGenome<T> &, p1);
		const GA2DArrayGenome<T> &dad =
			DYN_CAST(const GA2DArrayGenome<T> &, p2);

		int nc = 0;
		int i, j;

		if (c1 && c2)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);
			GA2DArrayGenome<T> &bro = DYN_CAST(GA2DArrayGenome<T> &, *c2);

			if (sis.width() == bro.width() && sis.height() == bro.height() &&
				mom.width() == dad.width() && mom.height() == dad.height() &&
				sis.width() == mom.width() && sis.height() == mom.height())
			{
				int count = 0;
				for (i = sis.width() - 1; i >= 0; i--)
				{
					for (j = sis.height() - 1; j >= 0; j--)
					{
						sis.gene(i, j,
								 ((count % 2 == 0) ? mom.gene(i, j)
												   : dad.gene(i, j)));
						bro.gene(i, j,
								 ((count % 2 == 0) ? dad.gene(i, j)
												   : mom.gene(i, j)));
						count++;
					}
				}
			}
			else
			{
				int startx, starty;
				int minx =
					(mom.width() < dad.width()) ? mom.width() : dad.width();
				int miny =
					(mom.height() < dad.height()) ? mom.height() : dad.height();
				startx = (sis.width() < minx) ? sis.width() : minx;
				starty = (sis.height() < miny) ? sis.height() : miny;
				for (i = startx - 1; i >= 0; i--)
					for (j = starty - 1; j >= 0; j--)
						sis.gene(i, j,
								 (((i * starty + j) % 2 == 0)
									  ? mom.gene(i, j)
									  : dad.gene(i, j)));
				startx = (bro.width() < minx) ? bro.width() : minx;
				starty = (bro.height() < miny) ? bro.height() : miny;
				for (i = startx - 1; i >= 0; i--)
					for (j = starty - 1; j >= 0; j--)
						bro.gene(i, j,
								 (((i * starty + j) % 2 == 0)
									  ? dad.gene(i, j)
									  : mom.gene(i, j)));
			}

			nc = 2;
		}
		else if (c1)
		{
			GA2DArrayGenome<T> &sis = DYN_CAST(GA2DArrayGenome<T> &, *c1);

			if (mom.width() == dad.width() && mom.height() == dad.height() &&
				sis.width() == mom.width() && sis.height() == mom.height())
			{
				int count = 0;
				for (i = sis.width() - 1; i >= 0; i--)
				{
					for (j = sis.height() - 1; j >= 0; j--)
					{
						sis.gene(i, j,
								 ((count % 2 == 0) ? mom.gene(i, j)
												   : dad.gene(i, j)));
						count++;
					}
				}
			}
			else
			{
				int minx =
					(mom.width() < dad.width()) ? mom.width() : dad.width();
				int miny =
					(mom.height() < dad.height()) ? mom.height() : dad.height();
				minx = (sis.width() < minx) ? sis.width() : minx;
				miny = (sis.height() < miny) ? sis.height() : miny;
				for (i = minx - 1; i >= 0; i--)
					for (j = miny - 1; j >= 0; j--)
						sis.gene(i, j,
								 (((i * miny + j) % 2 == 0) ? mom.gene(i, j)
															: dad.gene(i, j)));
			}

			nc = 1;
		}

		return nc;
	}

  public:
	GA2DArrayGenome(unsigned int width, unsigned int height,
					GAGenome::Evaluator f = nullptr, void *u = nullptr)
		: GAArray<T>(width * height),
		  GAGenome(DEFAULT_2DARRAY_INITIALIZER, DEFAULT_2DARRAY_MUTATOR,
				   DEFAULT_2DARRAY_COMPARATOR)
	{
		evaluator(f);
		userData(u);
		crossover(DEFAULT_2DARRAY_CROSSOVER);
		nx = minX = maxX = width;
		ny = minY = maxY = height;
	}

	GA2DArrayGenome(const GA2DArrayGenome<T> &orig) : GAArray<T>(orig.sz)
	{
		GA2DArrayGenome<T>::copy(orig);
	}

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
	~GA2DArrayGenome() override{};

	void copy(const GAGenome &orig) override
	{
		if (&orig == this)
			return;
		const GA2DArrayGenome<T> *c =
			DYN_CAST(const GA2DArrayGenome<T> *, &orig);
		if (c)
		{
			GAGenome::copy(*c);
			GAArray<T>::copy(*c);
			nx = c->nx;
			ny = c->ny;
			minX = c->minX;
			minY = c->minY;
			maxX = c->maxX;
			maxY = c->maxY;
		}
	}

	void copy(const GA2DArrayGenome<T> &orig, unsigned int r, unsigned int s,
			  unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		if (w == 0 || x >= orig.nx || r >= nx || h == 0 || y >= orig.ny ||
			s >= ny)
			return;
		if (x + w > orig.nx)
			w = orig.nx - x;
		if (y + h > orig.ny)
			h = orig.ny - y;
		if (r + w > nx)
			w = nx - r;
		if (s + h > ny)
			h = ny - s;

		for (unsigned int j = 0; j < h; j++)
			GAArray<T>::copy(orig, (s + j) * nx + r, (y + j) * orig.nx + x, w);

		_evaluated = false;
	}

	GAGenome *clone(GAGenome::CloneMethod flag) const override
	{
		GA2DArrayGenome<T> *cpy = new GA2DArrayGenome<T>(nx, ny);
		if (flag == CloneMethod::CONTENTS)
		{
			cpy->copy(*this);
		}
		else
		{
			cpy->GAGenome::copy(*this);
			cpy->minX = minX;
			cpy->minY = minY;
			cpy->maxX = maxX;
			cpy->maxY = maxY;
		}
		return cpy;
	}

	virtual int resize(int w, int h)
	{
		if (w == STA_CAST(int, nx) && h == STA_CAST(int, ny))
			return this->size();

		if (w == GAGenome::ANY_SIZE)
			w = GARandomInt(minX, maxX);
		else if (w < 0)
			w = nx; // do nothing
		else if (minX == maxX)
			minX = maxX = w;
		else
		{
			if (w < STA_CAST(int, minX))
				w = minX;
			if (w > STA_CAST(int, maxX))
				w = maxX;
		}

		if (h == GAGenome::ANY_SIZE)
			h = GARandomInt(minY, maxY);
		else if (h < 0)
			h = ny; // do nothing
		else if (minY == maxY)
			minY = maxY = h;
		else
		{
			if (h < STA_CAST(int, minY))
				h = minY;
			if (h > STA_CAST(int, maxY))
				h = maxY;
		}

		if (w < STA_CAST(int, nx))
		{
			int y = GAMin(STA_CAST(int, ny), h);
			for (int j = 0; j < y; j++)
				GAArray<T>::move(j * w, j * nx, w);
		}
		GAArray<T>::size(w * h);
		if (w > STA_CAST(int, nx))
		{ // adjust the existing chunks of bits
			int y = GAMin(STA_CAST(int, ny), h);
			for (int j = y - 1; j >= 0; j--)
				GAArray<T>::move(j * w, j * nx, nx);
		}

		nx = w;
		ny = h;
		_evaluated = false;
		return this->size();
	}

	int read(std::istream &) override
	{
		GAErr(GA_LOC, className(), "read", GAError::OpUndef);
		return 1;
	}

	int write(std::ostream &os) const override
	{
		for (unsigned int j = 0; j < ny; j++)
		{
			for (unsigned int i = 0; i < nx; i++)
			{
				os << gene(i, j) << " ";
			}
			os << "\n";
		}
		return 0;
	}

	bool equal(const GAGenome &c) const override
	{
		if (this == &c)
			return true;
		GA2DArrayGenome<T> &b = (GA2DArrayGenome<T> &)c;
		if (nx != b.nx || ny != b.ny)
			return false;
		bool val = false;
		for (unsigned int j = 0; j < ny && val == 0; j++)
			val = GAArray<T>::equal(b, j * nx, j * nx, nx) ? false : true;
		return (val ? false : true);
	}

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

	int resizeBehaviour(GAGenome::Dimension which) const
	{
		int val = 0;
		if (which == Dimension::WIDTH)
		{
			if (maxX == minX)
				val = FIXED_SIZE;
			else
				val = maxX;
		}
		else if (which == Dimension::HEIGHT)
		{
			if (maxY == minY)
				val = FIXED_SIZE;
			else
				val = maxY;
		}
		return val;
	}

	int resizeBehaviour(GAGenome::Dimension which, unsigned int lower,
						unsigned int upper)
	{
		if (upper < lower)
		{
			GAErr(GA_LOC, className(), "resizeBehaviour",
				  GAError::BadResizeBehaviour);
			return resizeBehaviour(which);
		}

		switch (which)
		{
		case Dimension::WIDTH:
			minX = lower;
			maxX = upper;
			if (nx > upper)
				GA2DArrayGenome<T>::resize(upper, ny);
			if (nx < lower)
				GA2DArrayGenome<T>::resize(lower, ny);
			break;

		case Dimension::HEIGHT:
			minY = lower;
			maxY = upper;
			if (ny > upper)
				GA2DArrayGenome<T>::resize(nx, upper);
			if (ny < lower)
				GA2DArrayGenome<T>::resize(nx, lower);
			break;

		default:
			break;
		}

		return resizeBehaviour(which);
	}

	int resizeBehaviour(unsigned int lowerX, unsigned int upperX,
						unsigned int lowerY, unsigned int upperY)
	{
		return (resizeBehaviour(Dimension::WIDTH, lowerX, upperX) *
				resizeBehaviour(Dimension::HEIGHT, lowerY, upperY));
	}

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
	GADefineIdentity("GA2DArrayAlleleGenome", GAID::ArrayAlleleGenome2D);

	// this does not handle genomes with multiple allele sets!
	static void UniformInitializer(GAGenome &c)
	{
		GA2DArrayAlleleGenome<T> &child =
			DYN_CAST(GA2DArrayAlleleGenome<T> &, c);
		child.resize(GAGenome::ANY_SIZE, GAGenome::ANY_SIZE);
		for (int i = child.width() - 1; i >= 0; i--)
			for (int j = child.height() - 1; j >= 0; j--)
				child.gene(i, j, child.alleleset().allele());
	}

	static int FlipMutator(GAGenome &c, float pmut)
	{
		GA2DArrayAlleleGenome<T> &child =
			DYN_CAST(GA2DArrayAlleleGenome<T> &, c);

		if (pmut <= 0.0)
			return (0);

		float nMut = pmut * STA_CAST(float, child.size());
		if (nMut < 1.0)
		{ // we have to do a flip test on each bit
			nMut = 0;
			for (int i = child.width() - 1; i >= 0; i--)
			{
				for (int j = child.height() - 1; j >= 0; j--)
				{
					if (GAFlipCoin(pmut))
					{
						child.gene(i, j, child.alleleset().allele());
						nMut++;
					}
				}
			}
		}
		else
		{ // only flip the number of bits we need to flip
			for (int n = 0; n < nMut; n++)
			{
				int m = GARandomInt(0, child.size() - 1);
				int i = m % child.width();
				int j = m / child.width();
				child.gene(i, j, child.alleleset().allele());
			}
		}
		return (STA_CAST(int, nMut));
	}

  public:
	GA2DArrayAlleleGenome(unsigned int width, unsigned int height,
						  const GAAlleleSet<T> &s,
						  GAGenome::Evaluator f = nullptr, void *u = nullptr)
		: GA2DArrayGenome<T>(width, height, f, u)
	{
		aset = std::vector<GAAlleleSet<T>>(1);
		aset.at(0) = s;

		this->initializer(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_INITIALIZER);
		this->mutator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_MUTATOR);
		this->comparator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_COMPARATOR);
		this->crossover(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_CROSSOVER);
	}

	GA2DArrayAlleleGenome(unsigned int width, unsigned int height,
						  const GAAlleleSetArray<T> &sa,
						  GAGenome::Evaluator f = nullptr, void *u = nullptr)
		: GA2DArrayGenome<T>(width, height, f, u)
	{
		aset = std::vector<GAAlleleSet<T>>(sa.size());
		for (int i = 0; i < aset.size(); i++)
			aset.at(i) = sa.set(i);

		this->initializer(
			GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_INITIALIZER);
		this->mutator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_MUTATOR);
		this->comparator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_COMPARATOR);
		this->crossover(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_CROSSOVER);
	}

	GA2DArrayAlleleGenome(const GA2DArrayAlleleGenome<T> &orig)
		: GA2DArrayGenome<T>(orig.nx, orig.ny)
	{
		GA2DArrayAlleleGenome<T>::copy(orig);
	}

	~GA2DArrayAlleleGenome() { }

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

	GAGenome *clone(
		GAGenome::CloneMethod = GAGenome::CloneMethod::CONTENTS) const override
	{
		return new GA2DArrayAlleleGenome<T>(*this);
	}

	void copy(const GAGenome &orig) override
	{
		if (&orig == this)
			return;
		const GA2DArrayAlleleGenome<T> *c =
			DYN_CAST(const GA2DArrayAlleleGenome<T> *, &orig);
		if (c)
		{
			GA2DArrayGenome<T>::copy(*c);
			if (aset.size() != c->aset.size())
			{
				aset = std::vector<GAAlleleSet<T>>(c->aset.size());
			}
			for (std::size_t i = 0; i < aset.size(); i++)
				aset.at(i).link(c->aset.at(i));
		}
	}

	int read(std::istream &is) override { return GA2DArrayGenome<T>::read(is); }

	int write(std::ostream &os) const override
	{
		return GA2DArrayGenome<T>::write(os);
	}

	bool equal(const GAGenome &c) const override
	{
		return GA2DArrayGenome<T>::equal(c);
	}

	int resize(int x, int y) override
	{
		unsigned int oldx = this->nx;
		unsigned int oldy = this->ny;
		GA2DArrayGenome<T>::resize(x, y);

		if (this->nx > oldx)
		{ // adjust the existing chunks of bits
			int ynew = GAMin(oldy, this->ny);
			for (int j = ynew - 1; j >= 0; j--)
			{
				for (unsigned int i = oldx; i < this->nx; i++)
					this->a.at(j * this->nx + i) =
						aset.at((j * this->nx + i) % aset.size()).allele();
			}
		}
		if (this->ny > oldy)
		{ // change in height is always new bits
			for (unsigned int i = this->nx * oldy; i < this->nx * this->ny; i++)
				this->a.at(i) = aset.at(i % aset.size()).allele();
		}

		return this->size();
	}

	const GAAlleleSet<T> &alleleset(unsigned int i = 0) const
	{
		return aset.at(i % aset.size());
	}

  protected:
	std::vector<GAAlleleSet<T>> aset;
};

#endif
