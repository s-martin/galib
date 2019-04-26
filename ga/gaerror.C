// $Header$
/* ----------------------------------------------------------------------------
  error.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  This file contains all of the error messages for the library.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cstring>
#include <gaerror.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


static std::ostream *__gaErrStream = &std::cerr;
static bool __gaErrFlag = true;
static std::vector<std::string> __gaErrStr = {
	"error reading from file: ",
	"error writing to file: ",
	"unexpected EOF encountered during read.",
	"bad probability value.  Must be between 0.0 and 1.0, inclusive.",
	"objects are different types.",
	"this method has not been defined.",
	"core deleted with references remaining.",

	"the custom replacement strategy requires a replacement function",
	"unknown replacement strategy",
	"number of children must be greater than 0",
	"replacement percentage must be between 0.0 and 1.0, inclusive",
	"number of indiv for replacement must be less than pop size",
	"index of individual is out-of-bounds",
	"population contains no individuals from which to clone new individuals",
	"there must be at least one individual in each population",
	"no sexual crossover has been defined.  no mating can occur.",
	"no asexual crossover has been defined.  no mating can occur.",

	"children must have same resize behaviour for any given dimension",
	"parents and children must have the same dimensions",
	"parents must be the same length",
	"upper limit must be greater than lower limit.",
	"bad phenotype - ID is out of bounds.",
	"bad phenotype - value is less than min or greater than max.",
	"dimensions of bounds set do not match dimensions of the genome",

	"linear scaling multiplier must be greater than 1.0",
	"sigma truncation multiplier must be greater than 0.0",
	"negative objective function score!\n\
    all raw objective scores must be positive for linear scaling.",
	"negative objective function score!\n\
    all raw objective scores must be positive for power law scaling.",
	"the cutoff for triangular sharing must be greater than 0.0",

	"cannot index an allele in a bounded, non-discretized set of alleles",
	"length of binary string exceeds maximum for this computer/OS type.",
	"specified value cannot be exactly represented with these bits.",
	"bad 'where' indicator",
	"bogus type, data may be corrupt",
	"bad links in tree.  operation aborted.",
	"cannot swap a node with its ancestor",
	"cannot insert this object into itself",
	"node relative to which insertion is made must be non-NULL.",
	"root node must have no siblings.  insertion aborted.",
	"cannot insert before a root node (only below).",
	"cannot insert after a root node (only below)."};

void GAErr(const GASourceLocator loc, const std::string &clss,
		   const std::string &func, const std::string &msg1,
		   const std::string &msg2, const std::string &msg3)
{
	std::stringstream errstr;
	errstr << clss << "::" << func << ":" << std::endl;
	errstr << msg1 << std::endl;
	
	if (!msg2.empty())
	{
		errstr << "  " << msg2 << std::endl;
	}
	if (!msg3.empty())
	{
		errstr << "  " << msg3 << std::endl;
	}

	errstr << "  " << loc.file << " : " << loc.line << std::endl;
	
	gaErrMsg = errstr.str();

	if (__gaErrFlag == true)
	{
		*__gaErrStream << gaErrMsg;
	}
}

void GAErr(const GASourceLocator loc, const std::string &clss,
		   const std::string &func,
		   GAErrorIndex i, const std::string &msg2, const std::string &msg3)
{
	std::stringstream errstr;
	errstr << clss << "::" << func << ":" << std::endl;
	errstr << __gaErrStr.at(i) << std::endl;

	if (!msg2.empty())
	{
		errstr << "  " << msg2 << std::endl;
	}
	if (!msg3.empty())
	{
		errstr << "  " << msg3 << std::endl;
	}
	
	errstr << "  " << loc.file << " : " << loc.line << std::endl;

	gaErrMsg = errstr.str();

	if (__gaErrFlag == true)
	{
		*__gaErrStream << gaErrMsg;
	}
}

void GAErr(const GASourceLocator loc, const std::string &func, GAErrorIndex i,
		   const std::string &msg2, const std::string &msg3)
{
	std::stringstream errstr;
	errstr << func << ":" << std::endl;
	errstr << __gaErrStr.at(i) << std::endl;

	if (!msg2.empty())
	{
		errstr << "  " << msg2 << std::endl;
	}
	if (msg3.empty())
	{
		errstr << "  " << msg3 << std::endl;
	}

	errstr << "  " << loc.file << " : " << loc.line << std::endl;
	
	gaErrMsg = errstr.str();

	if (__gaErrFlag == true)
	{
		*__gaErrStream << gaErrMsg;
	}
}

void GAReportErrors(bool flag) { __gaErrFlag = flag; }

void GASetErrorStream(std::ostream &s) { __gaErrStream = &s; }
