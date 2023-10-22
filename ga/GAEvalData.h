/* ----------------------------------------------------------------------------
  mbwall 3dec95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */

#pragma once

/**
 * This is the basic interface for the object that contains evaluation data. It
 * can be used with genomes and/or populations in combination with their
 * respective evaluation methods.
 */
class GAEvalData
{
  public:
	GAEvalData() = default;
	GAEvalData(const GAEvalData &) = default;
	virtual ~GAEvalData() = default;
	GAEvalData &operator=(const GAEvalData &orig)
	{
		if (&orig != this)
		{
			copy(orig);
		}
		return *this;
	}
	virtual GAEvalData *clone() const = 0;
	virtual void copy(const GAEvalData &) = 0;
};
