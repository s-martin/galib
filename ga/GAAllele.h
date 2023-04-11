/* ----------------------------------------------------------------------------
  mbwall 21mar95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

#include "gaconfig.h"
#include <garandom.h>

#include <GAAllele.h>
#include <cstring>
#include <gaerror.h>
#include <istream>
#include <ostream>
#include <vector>

constexpr auto GA_ALLELE_CHUNK = 10;

/** Allele class
 * 
 * An allele is a possible value for a gene and an allele set is a list of possible values (I use 'set' because it doesn't
 * imply the specific implementation of the container class).
 */
class GAAllele
{
  public:
	enum class Type
	{
		ENUMERATED = 1,
		BOUNDED = 2,
		DISCRETIZED
	};
	enum class BoundType
	{
		NONE,
		INCLUSIVE,
		EXCLUSIVE
	};
};

/* ----------------------------------------------------------------------------
  This object contains a set of alleles for use with a similarly typed genome.
  This object can be used with any type that has operator= defined for it.  If
you use the remove member then you must have operator== defined for it.
  This should be implemented as a derivative of the Array class?  But I don't
want that overhead at this point.  Also, behaviour is not the same.
  The allele set uses the envelope/message structure.  The core allele object
is a reference-counted structure that contains all of the guts of an allele
set.  The outer shell, the allele set itself, is what users actually see.  It
defines the interface.  With this setup you can create a single allele set then
each genome does not have to make its own copy.  And we don't have to worry
about an allele set going out of scope then leaving genomes hanging (a problem
if we just used a pointer to a single alleleset with no reference counting).
  You can link an allele set to another so that they share the same core.  Use
the 'link' member function (this is typically used within the GAlib to reduce
the number of actual alleleset instances when cloning populations of genomes).
  There is no way to 'resize' an allele set.  You must add to it or remove
elements from it.
  The base template class assumes that the objects in the allele set are
complex, i.e. it is not OK to do a bit-copy of each object.  We should do
specializations for non-complex objects (or perhaps a separate class that does
bit-copies rather than logical-copies?)
  When you clone an allele set, the new one has its own core.
  Why didn't I do this as a couple of objects (enumerated set, bounded set,
discretized set, etc)?  I wanted to be able to have an array that uses sets of
many different types.  I suppose the allele set should be completely
polymorphic like the rest of the GAlib objects, but for now we'll do it as
a single object with multiple personalities (and a state).
  There is no error checking.  You should check the type before you try to
call any of the member functions.  In particular, if you try to get the
bounds on an enumerated set of one element, it will break.

*** should the assignment operator check to be sure that no allele is
	duplicated, or is it OK to have duplicate alleles in a set?  For now we
	allow duplicates (via either the add or assignemnt ops).
---------------------------------------------------------------------------- */
template <class T> class GAAlleleSetCore
{
  public:
	GAAlleleSetCore()
		:  csz(GA_ALLELE_CHUNK), 
		  a(std::vector<T>())
	{
		lowerb = GAAllele::BoundType::NONE;
		upperb = GAAllele::BoundType::NONE;

		cnt = 1;
	}

	GAAlleleSetCore(unsigned int n, const T array[])
		: type(GAAllele::Type::ENUMERATED), csz(GA_ALLELE_CHUNK), sz(n),
		  SZ(GA_ALLELE_CHUNK)
	{
		while (SZ < sz)
			SZ += csz;
		a = std::vector<T>(SZ);
		for (unsigned int i = 0; i < sz; i++)
			a.at(i) = *(array + i);
		lowerb = GAAllele::BoundType::NONE;
		upperb = GAAllele::BoundType::NONE;

		cnt = 1;
	}

	GAAlleleSetCore(const T &lower, const T &upper,
					GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
					GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
		: type(GAAllele::Type::BOUNDED), csz(GA_ALLELE_CHUNK), sz(2), SZ(2),
		  a(std::vector<T>(2))
	{
		a.at(0) = lower;
		a.at(1) = upper;
		lowerb = lb;
		upperb = ub;

		cnt = 1;
	}

	GAAlleleSetCore(const T &lower, const T &upper, const T &increment,
					GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
					GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
		: type(GAAllele::Type::DISCRETIZED), csz(GA_ALLELE_CHUNK), sz(3), SZ(3),
		  a(std::vector<T>(3))
	{
		a.at(0) = lower;
		a.at(1) = upper;
		a.at(2) = increment;
		lowerb = lb;
		upperb = ub;

		cnt = 1;
	}

	// We do not copy the original's reference count!
	GAAlleleSetCore(const GAAlleleSetCore<T> &orig)
		: csz(orig.csz), sz(orig.sz), SZ(orig.SZ), a(std::vector<T>(orig.SZ))
	{
		for (unsigned int i = 0; i < sz; i++)
			a.at(i) = orig.a.at(i);
		lowerb = orig.lowerb;
		upperb = orig.upperb;
		type = orig.type;

		cnt = 1;
	}

	// This just trashes everything.  This shouldn't get called before the count
	// reaches zero, so if it does, we post an error.
	~GAAlleleSetCore()
	{
		if (cnt > 0)
			GAErr(GA_LOC, "GAAlleleSetCore", "destructor", GAError::RefsRemain);
	}

	// Copying the contents of another allele set core does NOT change the
	// current count of the allele set core!
	GAAlleleSetCore<T>& operator=(const GAAlleleSetCore<T> &orig)
	{
		if (this == &orig)
			return *this;

		if (SZ < orig.sz)
		{
			while (SZ < orig.sz)
				SZ += csz;
			a = std::vector<T>(SZ);
		}

		for (unsigned int i = 0; i < orig.sz; i++)
			a.at(i) = orig.a.at(i);

		sz = orig.sz;
		lowerb = orig.lowerb;
		upperb = orig.upperb;
		type = orig.type;

		return *this;
	}

	GAAllele::Type type{GAAllele::Type::ENUMERATED}; // is this an ennumerated or bounded set?
	GAAllele::BoundType lowerb, upperb; // what kind of limit is the bound?
	unsigned int cnt; // how many objects are using us?
	unsigned int csz; // how big are the chunks to allocate?
	unsigned int sz{0}; // number we have
	unsigned int SZ{0}; // how many have we allocated?
	std::vector<T> a;
};

template <class T> class GAAlleleSet
{
  public:
	GAAlleleSet() : core(nullptr) {}
	GAAlleleSet(unsigned int n, const T a[])
		: core(new GAAlleleSetCore<T>(n, a))
	{
	}
	GAAlleleSet(const T &lower, const T &upper,
				GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
				GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
		: core(new GAAlleleSetCore<T>(lower, upper, lb, ub))
	{
	}
	GAAlleleSet(const T &lower, const T &upper, const T &increment,
				GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
				GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
		: core(new GAAlleleSetCore<T>(lower, upper, increment, lb, ub))
	{
	}
	GAAlleleSet(const GAAlleleSet<T> &set)
		: core(new GAAlleleSetCore<T>(*(set.core)))
	{
	}
	virtual ~GAAlleleSet()
	{
		if (core != nullptr)
		{
			core->cnt -= 1;
			if (core->cnt == 0)
			{
				delete core;
			}
		}
	}
	GAAlleleSet<T> &operator=(const GAAlleleSet<T> &set)
	{
		if (this == &set)
		{
			return *this;
		}
		if (core)
		{
			*core = *set.core;
		}
		else
		{
			core = new GAAlleleSetCore<T>(*(set.core));
		}
		return *this;
	}
	GAAlleleSet<T> *clone() const { return new GAAlleleSet<T>(*this); }

	// When we link to another allele set, we point our core to that one.  Be
	// sure that we have a core.  If not, just point.  If so, trash as needed.
	// TODO check, if this wokrs now as it is now a std::vector
	void link(GAAlleleSet<T> set)
	{
		if (&set != this)
		{
			if (core != nullptr)
			{
				core->cnt -= 1;
				if (core->cnt == 0)
					delete core;
			}
			core = set.core;
			core->cnt += 1;
		}
	}

	void unlink()
	{
		if (core == nullptr)
			return; // nothing to unlink
		if (core->cnt > 1)
		{
			core->cnt -= 1;
			core = new GAAlleleSetCore<T>(*core);
		}
	}

	int size() const { return core->sz; } // only meaninful for enumerated sets

	// If everthing goes OK, return 0.  If there's an error, we return -1.  I
	// really wish there were enough compilers doing exceptions to use them...
	int add(const T &alle)
	{
		if (core == nullptr)
			core = new GAAlleleSetCore<T>;
		if (core->type != GAAllele::Type::ENUMERATED)
			return 1;
		if (core->sz >= core->SZ)
		{
			core->SZ += core->csz;
			auto tmp = core->a;
			core->a = std::vector<T>(core->SZ);
			for (unsigned int i = 0; i < core->sz; i++)
				core->a.at(i) = tmp.at(i);
		}
		core->a.at(core->sz) = alle;
		core->sz += 1;
		return 0;
	}

	int remove(const T &allele)
	{
		if (core == nullptr)
			core = new GAAlleleSetCore<T>;
		if (core->type != GAAllele::Type::ENUMERATED)
			return 1;
		for (unsigned int i = 0; i < core->sz; i++)
		{
			if (core->a[i] == allele)
			{
				for (unsigned int j = i; j < core->sz - 1; j++)
					core->a[j] = core->a[j + 1];
				//	memmove(&(core->a[i]), &(core->a[i+1]),
				//(core->sz-i-1)*sizeof(T));
				core->sz -= 1;
				i = core->sz; // break out of the loop
			}
		}
		return 0;
	}

	int remove(unsigned int x)
	{
		for (unsigned int j = x; j < core->sz - 1; j++)
			core->a[j] = core->a[j + 1];
		//  memmove(&(core->a[i]), &(core->a[i+1]), (core->sz-i-1)*sizeof(T));
		core->sz -= 1;
		return 0;
	}

	// When returning an allele from the set, we have to know what type we are.
	// The allele that we return depends on the type.  If we're an enumerated
	// set then just pick randomly from the list of alleles.  If we're a bounded
	// set then pick randomly from the bounds, and respect the bound types.  If
	// we're a discretized set then we do much as we would for the bounded set,
	// but we respect the discretization.
	//   Be sure to specialize this member function (see the real genome for an
	// example of how to do this)
	T allele() const
	{
		if (core->type == GAAllele::Type::ENUMERATED)
			return core->a[GARandomInt(0, core->sz - 1)];
		else if (core->type == GAAllele::Type::DISCRETIZED)
		{
			GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)",
				  GAError::OpUndef);
			return core->a[0];
		}
		else
		{
			GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)",
				  GAError::OpUndef);
			return core->a[0];
		}
	}

	// This works only for enumerated sets.  If someone tries to use this on a
	// non-enumerated set then we post an error message.  No bounds checking on
	// the value that was passed to us, but we do modulo it so that we'll never
	// break.  Also, this means you can wrap an allele set around an array that
	// is significantly larger than the allele set that defines its contents.
	T allele(unsigned int i) const
	{
		if (core->type == GAAllele::Type::ENUMERATED)
			return core->a[i % core->sz];
		else if (core->type == GAAllele::Type::DISCRETIZED)
		{
			GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)",
				  GAError::OpUndef);
			return core->a[0];
		}
		else
		{
			GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)",
				  GAError::NoAlleleIndex);
			return core->a[0];
		}
	}

	T lower() const { return core->a[0]; } // only for bounded sets
	T upper() const { return core->a[1]; }
	T inc() const { return core->a[2]; }
	GAAllele::BoundType lowerBoundType() const { return core->lowerb; }
	GAAllele::BoundType upperBoundType() const { return core->upperb; }
	GAAllele::Type type() const { return core->type; }

	int read(std::istream &)
	{
		GAErr(GA_LOC, "GAAlleleSet", "read", GAError::OpUndef);
		return 1;
	}

	int write(std::ostream &) const
	{
		GAErr(GA_LOC, "GAAlleleSet", "write", GAError::OpUndef);
		return 1;
	}

	template <class U>
	friend int operator==(const GAAlleleSet<U>&, const GAAlleleSet<U>&);
	template <class U>
	friend int operator!=(const GAAlleleSet<U>&, const GAAlleleSet<U>&);

  protected:
	GAAlleleSetCore<T> *core;
};

template <class T> class GAAlleleSetArray
{
  public:
	GAAlleleSetArray() : csz(GA_ALLELE_CHUNK),  aset(nullptr) {}

	GAAlleleSetArray(const GAAlleleSet<T> &s)
		: csz(GA_ALLELE_CHUNK), sz(1), SZ(GA_ALLELE_CHUNK),
		  aset(new GAAlleleSet<T> *[GA_ALLELE_CHUNK])
	{
		aset[0] = new GAAlleleSet<T>(s);
	}

	GAAlleleSetArray(const GAAlleleSetArray<T> &orig)
		: csz(orig.csz), sz(orig.sz), SZ(orig.SZ),
		  aset(new GAAlleleSet<T> *[orig.SZ])
	{
		for (unsigned int i = 0; i < sz; i++)
			aset[i] = new GAAlleleSet<T>(orig.set(i));
	}

	~GAAlleleSetArray()
	{
		for (unsigned int i = 0; i < sz; i++)
			delete aset[i];
		delete[] aset;
	}

	GAAlleleSetArray<T>& operator=(const GAAlleleSetArray<T> &orig)
	{
		if (this == &orig)
			return *this;
		unsigned int i;
		for (i = 0; i < sz; i++)
			delete aset[i];

		if (SZ < orig.sz)
		{
			while (SZ < orig.sz)
				SZ += csz;
			delete[] aset;
			aset = new GAAlleleSet<T> *[SZ];
		}
		for (i = 0; i < orig.sz; i++)
			aset[i] = new GAAlleleSet<T>(orig.set(i));

		sz = orig.sz;

		return *this;
	}

	int size() const { return sz; }
	const GAAlleleSet<T> &set(unsigned int i) const { return *(aset[i]); }

	// Return 0 if everything goes OK, otherwise return -1
	int add(const GAAlleleSet<T> &s)
	{
		if (sz + 1 > SZ)
		{
			SZ += csz;
			GAAlleleSet<T> **tmp = aset;
			aset = new GAAlleleSet<T> *[SZ];
			memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T> *));
			delete[] tmp;
		}
		aset[sz] = new GAAlleleSet<T>(s);
		sz++;
		return 0;
	}

	int add(unsigned int n, const T a[])
	{
		if (sz + 1 > SZ)
		{
			SZ += csz;
			GAAlleleSet<T> **tmp = aset;
			aset = new GAAlleleSet<T> *[SZ];
			memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T> *));
			delete[] tmp;
		}
		aset[sz] = new GAAlleleSet<T>(n, a);
		sz++;
		return 0;
	}

	int add(const T &lower, const T &upper,
			GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
			GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
	{
		if (sz + 1 > SZ)
		{
			SZ += csz;
			GAAlleleSet<T> **tmp = aset;
			aset = new GAAlleleSet<T> *[SZ];
			memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T> *));
			delete[] tmp;
		}
		aset[sz] = new GAAlleleSet<T>(lower, upper, lb, ub);
		sz++;
		return 0;
	}

	int add(const T &lower, const T &upper, const T &increment,
			GAAllele::BoundType lb = GAAllele::BoundType::INCLUSIVE,
			GAAllele::BoundType ub = GAAllele::BoundType::INCLUSIVE)
	{
		if (sz + 1 > SZ)
		{
			SZ += csz;
			GAAlleleSet<T> **tmp = aset;
			aset = new GAAlleleSet<T> *[SZ];
			memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T> *));
			delete[] tmp;
		}
		aset[sz] = new GAAlleleSet<T>(lower, upper, increment, lb, ub);
		sz++;
		return 0;
	}

	int remove(unsigned int n)
	{
		if (n >= sz)
			return 1;
		delete aset[n];
		for (unsigned int i = n; i < sz - 1; i++)
			aset[i] = aset[i + 1];
		sz--;
		return 0;
	}

  protected:
	unsigned int csz;
	unsigned int sz{0};
	unsigned int SZ{0};
	GAAlleleSet<T> **aset;
};

// could do these with a memcmp if the type is simple...
template <class T>
int operator==(const GAAlleleSet<T>& a, const GAAlleleSet<T>& b)
{
	if (a.core == b.core)
		return 1;
	if (a.core == 0 || b.core == 0)
		return 0;
	if (a.core->sz != b.core->sz)
		return 0;
	// return((memcmp(a.core, b.core, a.core->sz * sizeof(T)) == 0) ? 1 :
	// 0);
	unsigned int i;
	for (i = 0; i < a.core->sz && a.core->a[i] == b.core->a[i]; i++)
		;
	return ((i == a.core->sz) ? 1 : 0);
}

template <class T>
int operator!=(const GAAlleleSet<T>& a, const GAAlleleSet<T>& b)
{
	if (a.core == b.core)
		return 0;
	if (a.core == 0 || b.core == 0)
		return 1;
	if (a.core->sz != b.core->sz)
		return 1;
	// return((memcmp(a.core, b.core, a.core->sz * sizeof(T)) == 0) ? 0 :
	// 1);
	unsigned int i;
	for (i = 0; i < a.core->sz && a.core->a[i] == b.core->a[i]; i++)
		;
	return ((i == a.core->sz) ? 0 : 1);
}

template <class T>
std::ostream &operator<<(std::ostream &os, const GAAlleleSet<T> &arg)
{
	arg.write(os);
	return os;
}
template <class T>
std::istream &operator>>(std::istream &is, GAAlleleSet<T> &arg)
{
	arg.read(is);
	return is;
}
