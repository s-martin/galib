// $Header$
/* ----------------------------------------------------------------------------
  binstr.h
  mbwall 30jun95
  Copyright (c) 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This header defines the interface for the binary string.  This is a crude
version of a real bitstring object.  We don't do real bitstring in the
interest of speed and ease of coding this mess up.

 TO DO:
  we can get major improvements to speed by inlining functions and getting rid
of the error checking...  for example, inlining genome and removing the
conditional makes it go from 7.5 seconds to 3.2 seconds (bm bl cs 1024 c 0.9)
---------------------------------------------------------------------------- */
#pragma once


#include <cstring>
#include <garandom.h>
#include <gatypes.h>
#include <vector>


class GABinaryString
{
  public:
	explicit GABinaryString(unsigned int s)
	{
		resize(s);
	}

	/** Copy the contents of the bitstream.
     * 
     * We don't care what format it is in -
	 * we resize to make sure we have adequate space then we just copy all of the
	 * data.
	 * If the original is actually this, then we don't do anything.  If the
	 * original is not the same class as this, then we post an error and return.
	 * 
	 * @param orig 
	 */
	void copy(const GABinaryString &orig)
	{
		data = orig.data;
	}

	/** Resize the bitstream to the specified number of bits.
     * 
     * We return the number of bits actually allocated. The new allocated space not used
     * is set to zeros.
	 * 
	 * @param x desired size [bits]
	 * @return the new size
	 */
	int resize(unsigned int x)
	{
		data.resize(x);
		return data.size();
	}

	int size() const { return data.size(); }

	short bit(unsigned int a) const { return (data[a]); }

	short bit(unsigned int a, short val)
	{ // set/unset the bit
		return (data[a] = (val != 0 ? 1 : 0));
	}

	/** Are two (subset) bitstreams equal?
	 * 
	 * @param rhs Right hand side bitstream to compare
	 * @param lhsIdx Start index of left hand side bitstream
	 * @param rhsIdx Start index of right hand side bitstream
	 * @param l length of bitstream to compare
	 * @return True, if equal 
	 */
	bool equal(const GABinaryString &rhs, unsigned int lhsIdx, unsigned int rhsIdx, unsigned int l) const
	{
		return (std::memcmp(&data[lhsIdx], &rhs.data[rhsIdx], l) != 0 ? false : true);
	}

	/**
	 * @brief 
	 * 
	 * @param orig
	 * @param dest destination
	 * @param src source
	 * @param l length
	 */
	void copy(const GABinaryString &orig, unsigned int dest, unsigned int src, unsigned int l)
	{
		data.resize(orig.data.size());
		std::memcpy(&data[dest], &orig.data[src], l);
	}

	/**
	 * @brief 
	 * 
	 * @param dest destination
	 * @param src source
	 * @param l length
	 */
	void move(unsigned int dest, unsigned int src, unsigned int l)
	{
		std::memmove(&data[dest], &data[src], l);
	}

	void set(unsigned int a, unsigned int l)
	{
		for (unsigned int i = a; i < l; i++)
		{
			data.at(i) = 1;
		}
	}

	void unset(unsigned int a, unsigned int l)
	{
				for (unsigned int i = a; i < l; i++)
		{
			data.at(i) = 0;
		}
	}

	void randomize(unsigned int a, unsigned int l)
	{
		for (unsigned int i = 0; i < l; i++)
		{
			data[i + a] = static_cast<GABit>(GARandomBit());
		}
	}

	void randomize()
	{
		for (auto &item : data)
		{
			item = static_cast<GABit>(GARandomBit());
		}
	}

  protected:
	/// the data themselves
	std::vector<GABit> data; 
};
