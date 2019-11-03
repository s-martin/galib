// $Header$
/* ----------------------------------------------------------------------------
  binstr1.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

#include <vector>

class GAMask
{
public:
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
	// TODO check, if using vector<bool> is feasible
	std::vector<char> _mask;
};
