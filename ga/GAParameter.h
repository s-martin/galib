// $Header$
/* ----------------------------------------------------------------------------
  parameters.h
  mbwall 14jul95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  Header for the parameters object used by the GA objects.
---------------------------------------------------------------------------- */
#ifndef _ga_parameters_h_
#define _ga_parameters_h_

#include <gaconfig.h>
#include <gatypes.h>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <istream>
#include <list>
#include <optional>
#include <ostream>
#include <sstream>
#include <iostream>
#include <string>

// When specifying parameters for a GAlib object, you can use the fullname (the
// name used in parameters data files).  When specifying parameters in your code
// you can use a string, or use the predefined macros below.
constexpr auto gaNnGenerations = "number_of_generations";
constexpr auto gaNpConvergence = "convergence_percentage";
constexpr auto gaNnConvergence = "generations_to_convergence";
constexpr auto gaNpCrossover = "crossover_probability";
constexpr auto gaNpMutation = "mutation_probability";
constexpr auto gaNpopulationSize = "population_size";
constexpr auto gaNnPopulations = "number_of_populations";
constexpr auto gaNpReplacement = "replacement_percentage";
constexpr auto gaNnReplacement = "replacement_number";
constexpr auto gaNnBestGenomes = "number_of_best";
constexpr auto gaNscoreFrequency = "score_frequency";
constexpr auto gaNflushFrequency = "flush_frequency";
constexpr auto gaNscoreFilename = "score_filename";
constexpr auto gaNselectScores = "select_scores";
constexpr auto gaNelitism = "elitism";
constexpr auto gaNnOffspring = "number_of_offspring";
constexpr auto gaNrecordDiversity = "record_diversity";
constexpr auto gaNpMigration = "migration_percentage";
constexpr auto gaNnMigration = "migration_number";
constexpr auto gaNminimaxi = "minimaxi";
constexpr auto gaNseed = "seed";

/* ----------------------------------------------------------------------------
   The parameter list is implemented as an array, but has the interface of a
list.  Don't ask.  You can traverse through the list to get the parameters that
you need.  Be sure to check the type before you try to extract the value for
any specific parameter in the list.
---------------------------------------------------------------------------- */
class GAParameterList
{
  public:
	GAParameterList();

	template <typename T> 
	bool set(const std::string &name, T value)
	{
		try
		{
			m_vm.at(name).value() = value;
			return true;
		}
		catch (std::out_of_range &e)
		{
			std::cerr << "Could not set value for " << name << ". Exception: " << e.what() << std::endl;
		}
		return false;
	}

	bool set(const std::string &name, int v) { return set<int>(name, v); }
	bool set(const std::string &name, unsigned int v) {	return set<unsigned int>(name, v); }
	bool set(const std::string &name, char v) { return set<char>(name, v); }
	bool set(const std::string &name, const char *v)
	{
		std::string str(v);
		return set<std::string>(name, str);
	}
	bool set(const std::string &name, double v) { return set<double>(name, v); }
	bool set(const std::string &name, float v) { return set<float>(name, v); }
	bool parse(int &argc, char **argv);

	bool write(const char *filename) const;
	bool write(std::ostream &os) const;
	bool read(const std::string &filename);
	bool read(std::istream &is);

	unsigned int numGenerations;
	float convergencePercentage;
	unsigned int generationsToConvergence;
	float probCrossover;
	float probMutation;
	unsigned int populationSize;
	unsigned int numberOfBest;
	unsigned int scoreFrequency;
	unsigned int flushFrequency;
	std::string scoreFilename;
	int selectScores;
	bool recordDiversity;
	float migrationPercentage;
	int miniMaxi;
	int seed;
	unsigned int numPopulations;
	unsigned int migrationNumber;
	unsigned int numOffspring;
	bool elitism;
	float replacementPercentage;
	int numPercentage;

  private:
	  boost::program_options::options_description options();

	template <class charT>
	boost::program_options::basic_parsed_options<charT> parseGAlibSettingsFile(std::basic_istream<charT> &is, 
		const boost::program_options::options_description &desc, bool allow_unregistered = false)
	{
		// do any option check here

		// TODO check, if bool 1 / 0 is parsed here

		boost::program_options::parsed_options result(&desc);

		std::string line;
		std::vector<std::string> lines;
		while (std::getline(is, line))
		{
			if (!line.empty() && !boost::starts_with(line, "#"))
			{
				lines.emplace_back(line);
			}
		}

		for (const auto &l : lines)
		{
			std::vector<std::string> kvpair;
			boost::split(kvpair, l, boost::is_any_of("\t "));

			kvpair.erase(
				std::remove_if(kvpair.begin(), kvpair.end(),
							   [](const std::string &s) { return s.empty(); }),
				kvpair.end());

			if (kvpair.empty())
			{
				continue;
			}

			boost::program_options::basic_option<charT> opt;
			opt.string_key = kvpair.at(0);
			opt.value.push_back(kvpair.at(1));
			// TODO works not yet, because description is null
			opt.unregistered = (result.description->find_nothrow(kvpair.at(0), false) == nullptr);
			opt.position_key = -1;
			result.options.push_back(opt);
		}

		return boost::program_options::basic_parsed_options<charT>(result);
	};

	boost::program_options::variables_map m_vm;
	std::shared_ptr<boost::program_options::options_description> m_optionsDesc;
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

#endif
