// $Header$
/* ----------------------------------------------------------------------------
  binstr1.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_mask_h_
#define _ga_mask_h_

#include <cstring>
#include <vector>

class GAMask
{
public:
	GAMask()
	{
		_n = 0;
	}
	GAMask(const GAMask& m)
	{
		_n = 0;
		copy(m);
	}
	GAMask& operator=(const GAMask& m)
	{
		copy(m);
		return *this;
	}
	~GAMask() = default;
	void copy(const GAMask& m)
	{
		size(m.size());
		_mask = m._mask;
	}

	void clear()
	{
		_mask.assign(_n, 0);
	}
	int size() const { return _n; }
	int size(unsigned int s)
	{
		if (s > _n)
		{
			_n = s;
			_mask.assign(_n, 0);
		}
		return _n;
	}
	char mask(unsigned int i) const { return _mask.at(i); }
	char& mask(unsigned int i) { return _mask.at(i); }
	char operator[](unsigned int i) const { return _mask.at(i); }
	char& operator[](unsigned int i) { return _mask.at(i); }

protected:
	std::vector<char> _mask;
	unsigned int _n;
};

#endif
