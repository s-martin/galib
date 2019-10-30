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
	}
	GAMask(const GAMask& m)
	{
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
		_mask.assign(_mask.size(), 0);
	}
	int size() const { return _mask.size(); }
	int size(unsigned int s)
	{
		if (s > _mask.size())
		{
			_mask.assign(s, 0);
		}
		return _mask.size();
	}
	char mask(unsigned int i) const { return _mask.at(i); }
	char& mask(unsigned int i) { return _mask.at(i); }
	char operator[](unsigned int i) const { return _mask.at(i); }
	char& operator[](unsigned int i) { return _mask.at(i); }

protected:
	std::vector<char> _mask;
};

#endif
