// $Header$
/* ----------------------------------------------------------------------------
  parameters.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  Definition of the parameter object and a container list for it.  I did this
as a separate list implementation because I don't want all of the overhead of
a fullblown list.  The parameter list is a special purpose, stripped down list
implementation.
---------------------------------------------------------------------------- */
#include <GAParameter.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <gaconfig.h>
#include <gaerror.h>

#include <fstream>

#include <boost/algorithm/string.hpp>

//constexpr int PRM_CHUNKSIZE = 10;
constexpr int BUFSIZE = 1024; // size of buffer for reading pairs
constexpr int MAX_PAIRS = 5000; // max number of name-value pairs in stream
constexpr int NAMESIZE = 128; // max length of name in name-value pair

static bool IsNumeric(const char *);

GAParameter::GAParameter(const std::string &fn, const std::string &sn,
						 ParType tp,
						 const void *v)
{
	fname = fn;
	sname = sn;

	t = tp;
	memset(&val, 0, sizeof(Value));
	setvalue(v);
}

GAParameter::GAParameter(const GAParameter &orig)
{
	fname = sname = "";
	memset(&val, 0, sizeof(Value));
	copy(orig);
}

void GAParameter::copy(const GAParameter &orig)
{
	if (&orig == this)
	{
		return;
	}

	fname = orig.fname;
	sname = orig.sname;
	t = orig.t;
	setvalue(orig.value()); // do this directly...
}

GAParameter::~GAParameter()
{
	if (t == ParType::STRING)
	{
		delete[] val.sval;
	}
}

void GAParameter::setvalue(const void *v)
{
	switch (t)
	{
	case ParType::BOOLEAN:
	case ParType::INT:
		val.ival = *((int *)v);
		break;
	case ParType::CHAR:
		val.cval = *((char *)v);
		break;
	case ParType::STRING:
		if (v != val.sval)
		{
			char *ptr = nullptr;
			if (strlen((char *)v) > 0)
			{
				ptr = new char[strlen((char *)v) + 1];
				strcpy(ptr, (char *)v);
			}
			delete[] val.sval;
			val.sval = ptr;
		}
		break;
	case ParType::FLOAT:
		val.fval = *((float *)v);
		break;
	case ParType::DOUBLE:
		val.dval = *((double *)v);
		break;
	case ParType::POINTER:
	default:
		val.pval = v;
		break;
	}
}

// Set the specified parameter (if we have it).  If we don't recognize the name
// (ie it has not been added to the list) then we return the error code.
bool GAParameterList::set(const std::string &name, const void *v)
{
	auto param = std::find_if(this->begin(), this->end(),
							  [=](const GAParameter &p) -> bool {
								  return (name == p.fullname() || name == p.shrtname());
							  });

	if (param != this->end())
	{
		param->value(v);
		return true;
	}
	return false;
}

// Must do a special case for double/float.  Any floats that get passed to this
// routine will be cast to doubles, but then we need to force them back to a
// float if FLOAT is the type that is expected.  Kind of sucks, eh?
//   We could check the parameter type against the type here, but we don't.
// (could do it for all of the 'set' members).  Maybe in a later release.
bool GAParameterList::set(const std::string &name, double v)
{
	auto param = std::find_if(this->begin(), this->end(),
							  [=](const GAParameter &p) -> bool {
									return (name == p.fullname() || name == p.shrtname());
							  });

	if (param != this->end())
	{
		if (param->type() == ParType::FLOAT)
		{
			auto fval = static_cast<float>(v);
			param->value((void *)&fval);
		}
		else if (param->type() == ParType::DOUBLE)
		{
			param->value((void *)&v);
		}
		else
		{
			GAErr(GA_LOC, "GAParameterList", "set", GAError::BadTypeIndicator);
		}
		return true;
	}
	return false;
}

// This allocates space for strings, so be sure to free it!
bool GAParameterList::get(const std::string &name, void *value) const
{
	auto param = std::find_if(this->begin(), this->end(),
							  [=](const GAParameter &p) -> bool {
									return (name == p.fullname() || name == p.shrtname());
							  });

	if (param != this->end())
	{
		switch (param->type())
		{
		case ParType::BOOLEAN:
		case ParType::INT:
			*(static_cast<int *>(value)) = *((int *)param->value());
			break;
		case ParType::CHAR:
			*(static_cast<char *>(value)) = *((char *)param->value());
			break;
		case ParType::STRING:
			break;
		case ParType::FLOAT:
			*(static_cast<float *>(value)) = *((float *)param->value());
			break;
		case ParType::DOUBLE:
			*(static_cast<double *>(value)) = *((double *)param->value());
			break;
		case ParType::POINTER:
		default:
			break;
		}
		return true;
	}
	return false;
}

// Add the item to the list if it does not already exist.  Return 0 if the add
// was OK, -1 if there was a problem.
bool GAParameterList::add(const std::string &fn, const std::string &sn,
						  ParType t,
						  const void *v)
{
	auto param = std::find_if(this->begin(), this->end(),
							  [=](const GAParameter &p) -> bool {
									return (fn == p.fullname() && sn == p.shrtname());
							  });

	if (param == this->end())
	{
		this->emplace_back(GAParameter(fn, sn, t, v));
	}
	return true;
}

// Dump the parameters to the specified stream.  Just name-value pairs with a
// tab delimiter and newline separating pairs.
//   If there is an error, return 1, otherwise return 0.
bool GAParameterList::write(std::ostream &os) const
{
	for (auto p : *this)
	{
			int ival;
		float fval;
		double dval;
		char cval;
		char *sval;

		os << p.fullname() << "\t";

		switch (p.type())
		{
		case ParType::BOOLEAN:
			ival = *((int *)(p.value()));
			if (ival != 0)
			{
				os << "1\n";
			}
			else
			{
				os << "0\n";
			}
			break;
		case ParType::INT:
			ival = *((int *)(p.value()));
			os << ival << "\n";
			break;
		case ParType::CHAR:
			cval = *((char *)(p.value()));
			os << cval << "\n";
			break;
		case ParType::STRING:
			sval = ((char *)(p.value()));
			os << sval << "\n";
			break;
		case ParType::FLOAT:
			fval = *((float *)(p.value()));
			os << fval << "\n";
			break;
		case ParType::DOUBLE:
			dval = *((double *)(p.value()));
			os << dval << "\n";
			break;
		case ParType::POINTER:
		default:
			os << "(pointer)\n";
			//      os << p[i]->value() << "\n";
			break;
		}
	}
	return true;
}

bool GAParameterList::write(const char *filename) const
{
	std::ofstream outfile(filename, (std::ios::out | std::ios::trunc));
	// should be done this way, but SGI systems (and others?) don't do it
	// right...
	//  if(! outfile.is_open()){
	if (outfile.fail())
	{
		GAErr(GA_LOC, "GAParameterList", "write", GAError::WriteError, filename);
		return false;
	}
	bool status = write(outfile);
	outfile.close();
	return status;
}

// Read name-value pairs from the stream.  If the first item is a number, then
// we expect that many name-value pairs.  If not, then we read until the end of
// the stream.  Be sure not to duplicate the last one, and be sure to dump any
// items from a # to end-of-line.
//   Parse for name-value pairs, where the pairs are separated by whitespace.
// *Every* parameter must be specified as a name-value pair (no single name
// with no value allowed).  If we get a single name with no value that will
// screw up the name-value pairing for any remaining pairs.  We could check for
// this situation, but for now we just let it happen and dump the errors. (i'll
// do a better parsing algorithm at some later point).
//   We return the number of items that we were able to read from the stream.
// If we come across an item that is unrecognized, we dump an error message.
//   The buffer length is limited here, so don't try to read in any really long
// lines.
//   We parse only for items that we know about - if we come across a pair
// whose name we do not know about, we ignore the pair and go on to the next.
// There's no global list to tell us what type things are, and we don't assume.
//   We don't allow setting pointers using this method.
bool GAParameterList::read(std::istream &is, bool)
{
	if (this->empty())
	{
		return false;
	}

	char buf[BUFSIZE];
	char name[NAMESIZE];
	int toggle = 0, count = 0, npairs = -1;
	int nfound = 0;
	is.width(sizeof(buf)); // don't allow to overrun buffer

	do
	{
		is >> buf;
		if (npairs == -1)
		{
			if (IsNumeric(buf))
			{
				npairs = atoi(buf);
				is >> buf;
			}
			else
			{
				npairs = MAX_PAIRS;
			}
		}
		if (is.eof())
		{
			break;
		}
		else if (buf[0] == '#')
		{ // dump to end-of-line
			is.get(buf, BUFSIZE); // fails on lines longer than BUFSIZE
		}
		else if (toggle == 0)
		{
			strcpy(name, buf);
			toggle = 1;
		}
		else if (toggle == 1)
		{
			count += 1;
			toggle = 0;

			auto param = std::find_if(
				this->begin(), this->end(), [=](const GAParameter &p) -> bool {
					return (name == p.fullname() || name == p.shrtname());
				});

			if (param != this->end())
			{
				int ival;
				float fval;
				double dval;

				switch (param->type())
				{
				case ParType::BOOLEAN:
				case ParType::INT:
					ival = atoi(buf);
					set(name, (void *)&ival);
					nfound += 1;
					break;
				case ParType::CHAR:
				case ParType::STRING:
					set(name, (void *)buf);
					nfound += 1;
					break;
				case ParType::FLOAT:
					fval = static_cast<float>(atof(buf));
					set(name, (void *)&fval);
					nfound += 1;
					break;
				case ParType::DOUBLE:
					dval = atof(buf);
					set(name, (void *)&dval);
					nfound += 1;
					break;
				case ParType::POINTER:
				default:
					break;
				}

				std::string str =
					"unrecognized variable name '" + std::string(name) + "'";
				GAErr(GA_LOC, "GAParameterList", "read", str);
			}
		}
	} while (!is.eof() && count < npairs);

	if (toggle == 1)
	{
		std::string str = "variable " + std::string(name) + " has no value";

		GAErr(GA_LOC, "GAParameterList", "read", str,
			  "be sure there is a newline at end of the file");

		is.clear(std::ios::badbit | is.rdstate());
	}

	return true;
}

bool GAParameterList::read(const std::string &filename, bool flag)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile)
	{
		GAErr(GA_LOC, "GAParameterList", "read", GAError::ReadError, filename);
		return false;
	}
	bool status = read(infile, flag);
	infile.close();
	return status;
}

// Parse the arglist for any recognized arguments.  If we find a string we
// know, then we set the value.  If we find a string we don't know then we
// don't do anything unless the flag is set.  If the flag is set then we
// complain about unknown arguments.
//   You should set up the list before you do the parsing in order to grab
// arguments from the arglist.
//   When we encounter names we know with valid values, we put the list into
// order as we get the new values.  When we recognize a pair, we pull the
// parameter from the list, set its value, then stick it at the end of the
// parameter list.  So if something turns up more than once, we'll remove-
// then-append it more than once and the ordering from argv will be properly
// maintained.
//   We assume that argv[0] is the name of the program, so we don't barf on
// it if it is not a recognized name.
bool GAParameterList::parse(int &argc, char *argv[], bool flag)
{
	if (this->empty())
	{
		return false;
	}

	char **argvout = new char *[argc];
	int argcu = argc - 1;
	int argcl = 0;
	int nfound = 0;

	for (int i = 0; i < argc; i++)
	{
		int found = 0;

		// Loop through all of the parameters to see if we got a match.  If
		// there is no value for the name, complain.  Otherwise, set the value.

		auto param = std::find_if(
			this->begin(), this->end(), [=](const GAParameter &p) -> bool {
				return (argv[i] == p.fullname() || argv[i] == p.shrtname());
			});

		if (param != this->end())
		{
			found = 1;
			argvout[argcu] = argv[i];
			argcu--;
			if (++i >= argc)
			{
				GAErr(GA_LOC, argv[0], argv[i - 1], " needs a value");
			}
			else
			{
				int ival;
				float fval;
				double dval;

				switch (param->type())
				{
				case ParType::BOOLEAN:
					if (IsNumeric(argv[i]))
					{
						ival = atoi(argv[i]);
						ival = (ival == 0) ? 0 : 1;
					}
					else
					{
						if (boost::iequals(argv[i], "true") ||
							boost::iequals(argv[i], "t"))
						{
							ival = 1;
						}
					}
					set(argv[i - 1], (void *)&ival);
					nfound += 1;
					break;
				case ParType::INT:
					ival = atoi(argv[i]);
					set(argv[i - 1], (void *)&ival);
					nfound += 1;
					break;
				case ParType::CHAR:
				case ParType::STRING:
					set(argv[i - 1], (void *)argv[i]);
					nfound += 1;
					break;
				case ParType::FLOAT:
					fval = static_cast<float>(atof(argv[i]));
					set(argv[i - 1], (void *)&fval);
					nfound += 1;
					break;
				case ParType::DOUBLE:
					dval = atof(argv[i]);
					set(argv[i - 1], (void *)&dval);
					nfound += 1;
					break;
				case ParType::POINTER:
				default:
					break;
				}

				// Now update the argv array and argc count to indicate we
				// understood this one

				argvout[argcu] = argv[i];
				argcu--;
			}
		}

		if (found == 0)
		{
			if (flag && i != 0)
			{
				std::string str = "unrecognized name " + std::string(argv[i]);

				GAErr(GA_LOC, "GAParameterList", "parse", str);
			}
			argvout[argcl] = argv[i];
			argcl++;
		}
	}

	//  bcopy(argvout, argv, argc * sizeof(char*));
	memcpy(argv, argvout, argc * sizeof(char *));
	argc = argcl;
	delete[] argvout;

	return true;
}

static bool IsNumeric(const char *str)
{
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		if ((isdigit(str[i]) == 0) && str[i] != '.')
		{
			return true;
		}
	}
	return false;
}
