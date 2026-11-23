/* ----------------------------------------------------------------------------
  mbwall 25nov94
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  Shared definitions for the list objects.  The list itself is implemented by
GAList on top of std::list.
---------------------------------------------------------------------------- */

#pragma once

/** Constants shared by GAList and the genomes derived from it. */
class GAListBASE
{
  public:
	/** Values for 'where' to insert.
	 *
	 * The list is circular: BEFORE the head node inserts at the end of the list
	 * and leaves the head unchanged.
	 */
	enum Location
	{
		HEAD = 0,
		TAIL,
		BEFORE,
		AFTER
	};
	/** Return codes */
	enum
	{
		NO_ERR = 0,
		ERR = -1
	};

  protected:
	GAListBASE() = default;
	~GAListBASE() = default;
};
