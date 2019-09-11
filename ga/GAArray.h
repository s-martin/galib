// $Header$
/* ----------------------------------------------------------------------------
  arraytmpl.h
  mbwall 22apr95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the array base class.  Be careful what you put into one of these
arrays!  This class can be used only on objects that have:

  a default constructor (takes no arguments)
  operator=
  operator==
  operator!=

In other words, use only primitive objects in this array (e.g. int, float,
pointers, etc)

The constructor will try to initialize to zero, but only if the type is right.

We don't do any over-allocation, so resizing can be expensive.
No error checking on the copy, so don't walk over end of array!

TODO:
  should do specialization for simple types that does memcpy rather than loop
---------------------------------------------------------------------------- */
#ifndef _ga_arraytmpl_h_
#define _ga_arraytmpl_h_

#include <vector>


template <class T> class GAArray
{
  public:
	explicit GAArray(unsigned int s) : a(s)
	{
	}

	GAArray(const GAArray<T> &orig)
	{
		a.clear();
		copy(orig);
	}

	GAArray<T> &operator=(const GAArray<T> &orig)
	{
		copy(orig);
		return *this;
	}

	virtual ~GAArray() { }

	GAArray<T> *clone() { return new GAArray<T>(*this); }
	const T &operator[](unsigned int i) const { return a.at(i); }
	T &operator[](unsigned int i) { return a.at(i); }
	
	void copy(const GAArray<T> &orig)
	{
		a = std::vector(orig.a);
	}

	void copy(const GAArray<T> &orig, unsigned int dest, unsigned int src,
			  unsigned int length)
	{
		for (unsigned int i = 0; i < length; i++)
			a[dest + i] = orig.a[src + i];
	}

	void move(unsigned int dest, unsigned int src, unsigned int length)
	{
		if (src > dest)
			for (unsigned int i = 0; i < length; i++)
				a[dest + i] = a[src + i];
		else if (src < dest)
			for (unsigned int i = length - 1; i != 0; i--)
				a[dest + i] = a[src + i];
	}

	void swap(unsigned int i, unsigned int j)
	{
		auto tmp = a.at(j);
		a.at(j) = a.at(i);
		a.at(i) = tmp;
	}

	int size() const { return a.size(); }
	
	int size(unsigned int n)
	{
		if (n == a.size())
			return a.size();

		a.resize(n);
		return a.size();
	}
	
	bool equal(const GAArray<T> &b, unsigned int dest, unsigned int src,
			   unsigned int length) const
	{
		for (unsigned int i = 0; i < length; i++)
			if (a[dest + i] != b.a[src + i])
				return false;
		return true;
	}

  protected:
	// the contents of the array
	std::vector<T> a;
};

template <class T> int operator==(const GAArray<T> &a, const GAArray<T> &b)
{
	if (a.size() != b.size())
		return 0;
	return a.equal(b, 0, 0, a.size());
}
template <class T> int operator!=(const GAArray<T> &a, const GAArray<T> &b)
{
	if (a.size() != b.size())
		return 1;
	return a.equal(b, 0, 0, a.size()) ? 0 : 1;
}

#endif
