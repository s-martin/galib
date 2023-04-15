/* ----------------------------------------------------------------------------
  mbwall 14jul95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved
---------------------------------------------------------------------------- */
#pragma once

#include <gaconfig.h>
#include <gatypes.h>

#include <istream>
#include <list>
#include <ostream>

enum class ParType
{
	BOOLEAN,
	CHAR,
	STRING,
	INT,
	FLOAT,
	DOUBLE,
	POINTER
};

/** Naming the parameters
 * 
 * We associate a fullname, a short name, and a value with each parameter.
 * 
 */
class GAParameter
{
  public:
	GAParameter(const std::string &fn, const std::string &sn, ParType tp, const void *v);
	GAParameter(const GAParameter &orig);
	GAParameter &operator=(const GAParameter &orig)
	{
		copy(orig);
		return *this;
	}
	virtual ~GAParameter();
	void copy(const GAParameter &);
	std::string fullname() const { return fname; }
	std::string shrtname() const { return sname; }
	const void *value() const
	{
		return (t == ParType::STRING
					? val.sval
					: (t == ParType::POINTER ? val.pval : &val));
	}
	const void *value(const void *v)
	{
		setvalue(v);
		return (t == ParType::STRING
					? val.sval
					: (t == ParType::POINTER ? val.pval : &val));
	}
	ParType type() const { return t; }

  protected:
	std::string fname;
	std::string sname;
	union Value {
		int ival;
		char cval;
		char *sval;
		float fval;
		double dval;
		const void *pval;
	} val;
	ParType t;
	void setvalue(const void *);
};

/** Parameter List
 * 
 * The parameter list is implemented as an array, but has the interface of a
 * list.  You can traverse through the list to get the parameters that you need.
 * Be sure to check the type before you try to extract the value for any specific parameter in the list.
 * 
 */
class GAParameterList : public std::list<GAParameter>
{
  public:
	bool get(const std::string &name, void *) const;
	bool set(const std::string &name, const void *);
	bool set(const std::string &name, int v) { return set(name, (void *)&v); }
	bool set(const std::string &name, unsigned int v) { return set(name, (void *)&v); }
	bool set(const std::string &name, char v) { return set(name, (void *)&v); }
	bool set(const std::string &name, const char *v) { return set(name, (void *)v); }
	bool set(const std::string &name, double v);
	bool add(const std::string &fn, const std::string &sn, ParType, const void *);
	bool parse(int &argc, char **argv, bool flag = true);

	bool write(const char *filename) const;
	bool write(std::ostream &os) const;
	bool read(const std::string &filename, bool flag = true);
	bool read(std::istream &is, bool flag = true);
};

inline std::ostream &operator<<(std::ostream &os, const GAParameterList &plist)
{
	plist.write(os);
	return os;
}
inline std::istream &operator>>(std::istream &is, GAParameterList &plist)
{
	plist.read(is);
	return is;
}
