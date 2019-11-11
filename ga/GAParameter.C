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

namespace po = boost::program_options;

constexpr int PRM_CHUNKSIZE = 10;
constexpr int BUFSIZE = 1024; // size of buffer for reading pairs
constexpr int MAX_PAIRS = 5000; // max number of name-value pairs in stream
constexpr int NAMESIZE = 128; // max length of name in name-value pair

GAParameterList::GAParameterList()
{
	auto opt = options();

	std::ifstream file("default.ini", std::ifstream::in);
	try
	{
		po::store(po::parse_config_file(file, opt), m_vm, true);
		po::notify(m_vm);
	}
	catch (po::unknown_option & e)
	{
		std::cerr << e.what()<< std::endl;
	}
	file.close();
}

po::options_description GAParameterList::options()
{
	po::options_description optionsDesc("All options");

	po::options_description mandatoryDesc("Mandatory Options");
	mandatoryDesc.add_options()
		("help", "produce help message")
		(gaNnGenerations, po::value<unsigned int>(&numGenerations)->default_value(250), "Number of generations")
		(gaNpConvergence, po::value<float>(&convergencePercentage)->default_value(0.99), "Convergence Percentage")
		(gaNnConvergence, po::value<unsigned int>(&generationsToConvergence)->default_value(20), "Generations to Convergence")
		(gaNpCrossover, po::value<float>(&probCrossover)->default_value(0.01), "crossover_probability")
		(gaNpMutation, po::value<float>(&probMutation)->default_value(0.90), "mutation_probability")
		(gaNpopulationSize, po::value<unsigned int>(&populationSize)->default_value(50), "population_size")
		(gaNnBestGenomes, po::value<unsigned int>(&numberOfBest)->default_value(1), "number_of_best")
		(gaNscoreFrequency, po::value<unsigned int>(&scoreFrequency)->default_value(1), "score_frequency")
		(gaNflushFrequency, po::value<unsigned int>(&flushFrequency)->default_value(0), "flush_frequency")
		(gaNscoreFilename, po::value<std::string>(&scoreFilename)->default_value("generations.dat"), "score_filename")
		(gaNselectScores, po::value<int>(&selectScores)->default_value(2), "select_scores")
		(gaNrecordDiversity, po::value<bool>(&recordDiversity)->default_value(false), "record_diversity")
		(gaNpMigration, po::value<float>(&migrationPercentage)->default_value(0.1), "migration_percentage")
		(gaNminimaxi, po::value<int>(&miniMaxi)->default_value(1), "minimaxi")
		(gaNseed, po::value<int>(&seed)->default_value(0), "seed");

	// deme
	po::options_description demeDesc("Deme Options");
	demeDesc.add_options()
		(gaNnPopulations, po::value<unsigned int>(&numPopulations)->default_value(10), "number_of_populations")
		(gaNnMigration, po::value<unsigned int>(&migrationNumber)->default_value(5), "migration_number");

	// Inc
	po::options_description incDesc("Incremental Options");
	incDesc.add_options()
		(gaNnOffspring, po::value<unsigned int>(&numOffspring)->default_value(2), "number_of_offspring");
	//	int gaDefScoreFrequency2 = 100;

	// simple
	po::options_description simpleDesc("Simple Options");
	simpleDesc.add_options()
		(gaNelitism, po::value<bool>(&elitism)->default_value(true), "elitism");

	// Steady State
	po::options_description steadystateDesc("Steady State Options");
	steadystateDesc.add_options()
		(gaNpReplacement, po::value<float>(&replacementPercentage)->default_value(0.5), "replacement_percentage")
		(gaNnReplacement, po::value<int>(&numPercentage)->default_value(10), "replacement_number");
	//	int gaDefScoreFrequency2 = 100;

	optionsDesc.add(mandatoryDesc).add(demeDesc).add(incDesc).add(simpleDesc).add(steadystateDesc);
	return optionsDesc;
}

bool GAParameterList::parse(int &argc, char **argv)
{
	auto opt = options();

	po::store(po::parse_command_line(argc, argv, opt), m_vm);
	po::notify(m_vm);

	return true;
}

// Dump the parameters to the specified stream.  Just name-value pairs with a
// tab delimiter and newline separating pairs.
//   If there is an error, return 1, otherwise return 0.
bool GAParameterList::write(std::ostream &os) const
{
	for (const auto &param : m_vm)
	{
		if (auto v = boost::any_cast<bool>(param.second.value()))
		{
			os << param.first.c_str() << "\t" << (v ? "1" : "0") << "\n";
		}
		else
		{
			os << param.first.c_str() << "\t" << param.second.as<std::string>() << "\n";
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
// we expect that many name-value pairs.  If not, then we read until the end
// of / the stream.  Be sure not to duplicate the last one, and be sure to dump
// any / items from a # to end-of-line. /   Parse for name-value pairs, where
// the pairs are separated by whitespace. / *Every* parameter must be specified
// as a name-value pair (no single name / with no value allowed).  If we get a
// single name with no value that will / screw up the name-value pairing for
// any remaining pairs.  We could check for / this situation, but for now we
// just let it happen and dump the errors. (i'll / do a better parsing
// algorithm at some later point). /   We return the number of items that we
// were able to read from the stream. / If we come across an item that is
// unrecognized, we dump an error message. /   The buffer length is limited
// here, so don't try to read in any really long / lines. /   We parse only for
// items that we know about - if we come across a pair / whose name we do not
// know about, we ignore the pair and go on to the next. / There's no global
// list to tell us what type things are, and we don't assume. /   We don't
// allow setting pointers using this method.
bool GAParameterList::read(std::istream &is, bool flag)
{
	store(parseGAlibSettingsFile(is, *m_optionsDesc), m_vm);
	notify(m_vm);

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
