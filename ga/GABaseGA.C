// $Header$
/* ----------------------------------------------------------------------------
  gabase.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

  Source file for the base genetic algorithm object.
---------------------------------------------------------------------------- */
#include <GABaseGA.h>
#include <cstdio>
#include <cstring>
#include <garandom.h>
#include <gaversion.h> // gets the RCS string in for ident purposes

#include <boost/algorithm/string.hpp>

// Here we assign the default values of the GAlib default parameters.
int gaDefNumGen = 250;
float gaDefPConv = 0.99;
int gaDefNConv = 20;
float gaDefPMut = 0.01;
float gaDefPCross = 0.90;
int gaDefPopSize = 50;
int gaDefNPop = 10;
float gaDefPRepl = 0.5;
int gaDefNRepl = 10;
int gaDefNumOff = 2;
float gaDefPMig = 0.1;
int gaDefNMig = 5;
int gaDefSelectScores = GAStatistics::Maximum;
int gaDefMiniMaxi = 1;
bool gaDefDivFlag = false;
bool gaDefElitism = true;
int gaDefSeed = 0;

// return the configuration string that identifies this build of the library.
static const char *rcsid = GALIB_LIBRARY_IDENTIFIER;
const char *GAConfig() { return rcsid; }

// Here are a few termination functions that you can use.  Terminators return
// true if the algorithm should finish, false otherwise.
bool GAGeneticAlgorithm::TerminateUponGeneration(GAGeneticAlgorithm &ga)
{
	return (ga.generation() < ga.nGenerations() ? false : true);
}

// If we are maximizing, then terminate when the convergence has exceeded the
// specified convergence.  If we are minimizing, then terminate when the
// convergence has dropped below the specified convergence.
bool GAGeneticAlgorithm::TerminateUponConvergence(GAGeneticAlgorithm &ga)
{
	bool val = false;
	if (ga.minimaxi() == GAGeneticAlgorithm::MINIMIZE)
	{
		if (ga.convergence() == 0 || ga.convergence() > ga.pConvergence())
		{
			val = false;
		}
		else
		{
			val = true;
		}
	}
	else
	{
		if (ga.convergence() < ga.pConvergence())
		{
			val = false;
		}
		else
		{
			val = true;
		}
	}
	return val;
}

// Use the ratio between the minimum and maximum to determine whether the
// population has converged.  This method will not work if the values cross
// zero!
// Note that this is significantly different than the definition (and the
// bug-laden implementation) that was in version of GAlib prior to 2.4.5.
//
// For historical purposes, here is the old definition of this method:
//
// Use the ratio of the population mean divided by the population maximum to
// determine whether the population has converged.  If we are maximizing, then
// check to see if the ratio exceeds the convergence.  If we are minimizing,
// then check to see if the ratio has dropped below the convergence.
bool GAGeneticAlgorithm::TerminateUponPopConvergence(GAGeneticAlgorithm &ga)
{
	bool val = false;

	if (ga.statistics().current(GAStatistics::Maximum) == 0)
	{
		return val;
	}

	float ratio = ga.statistics().current(GAStatistics::Minimum) /
				  ga.statistics().current(GAStatistics::Maximum);

	if (ga.minimaxi() == GAGeneticAlgorithm::MINIMIZE)
	{
		if (ratio <= ga.pConvergence())
		{
			val = true;
		}
		else
		{
			val = false;
		}
	}
	else
	{
		if (ratio >= ga.pConvergence())
		{
			val = true;
		}
		else
		{
			val = false;
		}
	}

	return val;
}

GAParameterList &
GAGeneticAlgorithm::registerDefaultParameters(GAParameterList &p)
{
	p.add(gaNseed, gaSNseed, ParType::INT, &gaDefSeed);
	p.add(gaNminimaxi, gaSNminimaxi, ParType::INT, &gaDefMiniMaxi);
	p.add(gaNnGenerations, gaSNnGenerations, ParType::INT, &gaDefNumGen);
	p.add(gaNnConvergence, gaSNnConvergence, ParType::INT, &gaDefNConv);
	p.add(gaNpConvergence, gaSNpConvergence, ParType::FLOAT, &gaDefPConv);
	p.add(gaNpCrossover, gaSNpCrossover, ParType::FLOAT, &gaDefPCross);
	p.add(gaNpMutation, gaSNpMutation, ParType::FLOAT, &gaDefPMut);
	p.add(gaNpopulationSize, gaSNpopulationSize, ParType::INT, &gaDefPopSize);
	p.add(gaNnBestGenomes, gaSNnBestGenomes, ParType::INT,
		  &gaDefNumBestGenomes);
	p.add(gaNscoreFrequency, gaSNscoreFrequency, ParType::INT,
		  &gaDefScoreFrequency1);
	p.add(gaNflushFrequency, gaSNflushFrequency, ParType::INT,
		  &gaDefFlushFrequency);
	p.add(gaNrecordDiversity, gaSNrecordDiversity, ParType::INT, &gaDefDivFlag);
	p.add(gaNscoreFilename, gaSNscoreFilename, ParType::STRING,
		  gaDefScoreFilename.c_str());
	p.add(gaNselectScores, gaSNselectScores, ParType::INT, &gaDefSelectScores);

	return p;
}

// When we create a GA, we stuff the parameters with the basics that will be
// needed by most genetic algorithms - num generations, p convergence, etc.
GAGeneticAlgorithm::GAGeneticAlgorithm(const GAGenome &g)
{
	pop = new GAPopulation(g, gaDefPopSize);
	pop->geneticAlgorithm(*this);

	ud = nullptr;
	cf = GAGeneticAlgorithm::DEFAULT_TERMINATOR;

	d_seed = gaDefSeed;
	params.add(gaNseed, gaSNseed, ParType::INT, &d_seed);

	minmax = gaDefMiniMaxi;
	params.add(gaNminimaxi, gaSNminimaxi, ParType::INT, &minmax);
	ngen = gaDefNumGen;
	params.add(gaNnGenerations, gaSNnGenerations, ParType::INT, &ngen);
	nconv = gaDefNConv;
	stats.nConvergence(nconv);
	params.add(gaNnConvergence, gaSNnConvergence, ParType::INT, &nconv);
	pconv = gaDefPConv;
	params.add(gaNpConvergence, gaSNpConvergence, ParType::FLOAT, &pconv);
	pcross = gaDefPCross;
	params.add(gaNpCrossover, gaSNpCrossover, ParType::FLOAT, &pcross);
	pmut = gaDefPMut;
	params.add(gaNpMutation, gaSNpMutation, ParType::FLOAT, &pmut);
	int psize = pop->size();
	params.add(gaNpopulationSize, gaSNpopulationSize, ParType::INT, &psize);

	stats.scoreFrequency(gaDefScoreFrequency1);
	params.add(gaNscoreFrequency, gaSNscoreFrequency, ParType::INT,
			   &gaDefScoreFrequency1);
	stats.flushFrequency(gaDefFlushFrequency);
	params.add(gaNflushFrequency, gaSNflushFrequency, ParType::INT,
			   &gaDefFlushFrequency);
	stats.recordDiversity(gaDefDivFlag);
	params.add(gaNrecordDiversity, gaSNrecordDiversity, ParType::INT,
			   &gaDefDivFlag);
	stats.scoreFilename(gaDefScoreFilename);
	params.add(gaNscoreFilename, gaSNscoreFilename, ParType::STRING,
			   gaDefScoreFilename.c_str());
	stats.selectScores(gaDefSelectScores);
	params.add(gaNselectScores, gaSNselectScores, ParType::INT,
			   &gaDefSelectScores);
	stats.nBestGenomes(g, gaDefNumBestGenomes);
	params.add(gaNnBestGenomes, gaSNnBestGenomes, ParType::INT,
			   &gaDefNumBestGenomes);

	scross = g.sexual();
	across = g.asexual();
}

GAGeneticAlgorithm::GAGeneticAlgorithm(const GAPopulation &p)
{
	pop = new GAPopulation(p);
	pop->geneticAlgorithm(*this);

	ud = nullptr;
	cf = GAGeneticAlgorithm::DEFAULT_TERMINATOR;

	d_seed = gaDefSeed;
	params.add(gaNseed, gaSNseed, ParType::INT, &d_seed);

	minmax = gaDefMiniMaxi;
	params.add(gaNminimaxi, gaSNminimaxi, ParType::INT, &minmax);
	ngen = gaDefNumGen;
	params.add(gaNnGenerations, gaSNnGenerations, ParType::INT, &ngen);
	nconv = gaDefNConv;
	stats.nConvergence(nconv);
	params.add(gaNnConvergence, gaSNnConvergence, ParType::INT, &nconv);
	pconv = gaDefPConv;
	params.add(gaNpConvergence, gaSNpConvergence, ParType::FLOAT, &pconv);
	pcross = gaDefPCross;
	params.add(gaNpCrossover, gaSNpCrossover, ParType::FLOAT, &pcross);
	pmut = gaDefPMut;
	params.add(gaNpMutation, gaSNpMutation, ParType::FLOAT, &pmut);
	int psize = pop->size();
	params.add(gaNpopulationSize, gaSNpopulationSize, ParType::INT, &psize);

	stats.scoreFrequency(gaDefScoreFrequency1);
	params.add(gaNscoreFrequency, gaSNscoreFrequency, ParType::INT,
			   &gaDefScoreFrequency1);
	stats.flushFrequency(gaDefFlushFrequency);
	params.add(gaNflushFrequency, gaSNflushFrequency, ParType::INT,
			   &gaDefFlushFrequency);
	stats.recordDiversity(gaDefDivFlag);
	params.add(gaNrecordDiversity, gaSNrecordDiversity, ParType::INT,
			   &gaDefDivFlag);
	stats.scoreFilename(gaDefScoreFilename);
	params.add(gaNscoreFilename, gaSNscoreFilename, ParType::STRING,
			   gaDefScoreFilename.c_str());
	stats.selectScores(gaDefSelectScores);
	params.add(gaNselectScores, gaSNselectScores, ParType::INT,
			   &gaDefSelectScores);
	stats.nBestGenomes(p.individual(0), gaDefNumBestGenomes);
	params.add(gaNnBestGenomes, gaSNnBestGenomes, ParType::INT,
			   &gaDefNumBestGenomes);

	scross = p.individual(0).sexual();
	across = p.individual(0).asexual();
}

GAGeneticAlgorithm::GAGeneticAlgorithm(const GAGeneticAlgorithm &ga)
	: stats(ga.stats), params(ga.params)
{
	pop = ga.pop->clone();
	pop->geneticAlgorithm(*this);

	cf = ga.cf;
	ud = ga.ud;
	ngen = ga.ngen;
	nconv = ga.nconv;
	pconv = ga.pconv;
	pcross = ga.pcross;
	pmut = ga.pmut;
	minmax = ga.minmax;
	scross = ga.scross;
	across = ga.across;
	d_seed = ga.d_seed;
}

GAGeneticAlgorithm::~GAGeneticAlgorithm() 
{ 
	delete pop; 
}

void GAGeneticAlgorithm::copy(const GAGeneticAlgorithm &ga)
{
	if (pop != nullptr)
	{
		pop->copy(*(ga.pop));
	}
	else
	{
		pop = ga.pop->clone();
	}
	pop->geneticAlgorithm(*this);

	stats = ga.stats;
	params = ga.params;

	cf = ga.cf;
	ud = ga.ud;
	ngen = ga.ngen;
	nconv = ga.nconv;
	pconv = ga.pconv;
	pcross = ga.pcross;
	pmut = ga.pmut;
	minmax = ga.minmax;
	scross = ga.scross;
	across = ga.across;
	d_seed = ga.d_seed;
}

const GAParameterList &
GAGeneticAlgorithm::parameters(const GAParameterList &list)
{
	for (const auto &p : list)
	{
		setptr(p.fullname(), p.value());
	}
	return params;
}

const GAParameterList &GAGeneticAlgorithm::parameters(int &argc, char **argv, bool flag)
{
	params.parse(argc, argv, flag); // get the args we understand
	for (const auto &p : params)
	{
		setptr(p.fullname(), p.value());
	}
	return params;
}

const GAParameterList &GAGeneticAlgorithm::parameters(const std::string &filename, bool flag)
{
	params.read(filename, flag);
	for (const auto &p : params)
	{
		setptr(p.fullname(), p.value());
	}
	return params;
}

const GAParameterList &GAGeneticAlgorithm::parameters(std::istream &is, bool flag)
{
	params.read(is, flag);
	for (const auto &p : params)
	{
		setptr(p.fullname(), p.value());
	}
	return params;
}

// Return 0 if everything is OK, non-zero if error.  If we did not set anything
// then we return non-zero (this is not an error, but we indicate that we did
// not do anything).
//   The set method must set both the GA's parameter and the value in the
// parameter list (kind of stupid to maintain two copies of the same data, but
// oh well).  The call to set on params is redundant for the times when this
// method is called *after* the parameter list has been updated, but it is
// necessary when this method is called directly by the user.
int GAGeneticAlgorithm::setptr(const std::string &name, const void *value)
{
	int status = 1;

	params.set(name, value); // redundant for some cases, but not others

	if (boost::equals(name, gaNnBestGenomes) ||
		boost::equals(name, gaSNnBestGenomes))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		nBestGenomes(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNpopulationSize) ||
			 boost::equals(name, gaSNpopulationSize))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		populationSize(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNminimaxi) ||
			 boost::equals(name, gaSNminimaxi))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		minimaxi(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNnGenerations) ||
			 boost::equals(name, gaSNnGenerations))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		nGenerations(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNpConvergence) ||
			 boost::equals(name, gaSNpConvergence))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((float *)value) << "'\n";
#endif
		pConvergence(*((float *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNnConvergence) ||
			 boost::equals(name, gaSNnConvergence))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		nConvergence(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNpCrossover) ||
			 boost::equals(name, gaSNpCrossover))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((float *)value) << "'\n";
#endif
		pCrossover(*((float *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNpMutation) ||
			 boost::equals(name, gaSNpMutation))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((float *)value) << "'\n";
#endif
		pMutation(*((float *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNscoreFrequency) ||
			 boost::equals(name, gaSNscoreFrequency))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		stats.scoreFrequency(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNflushFrequency) ||
			 boost::equals(name, gaSNflushFrequency))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		stats.flushFrequency(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNrecordDiversity) ||
			 boost::equals(name, gaSNrecordDiversity))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		stats.recordDiversity(*((int *)value) != 0 ? true : false);
		status = 0;
	}
	else if (boost::equals(name, gaNselectScores) ||
			 boost::equals(name, gaSNselectScores))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << *((int *)value) << "'\n";
#endif
		stats.selectScores(*((int *)value));
		status = 0;
	}
	else if (boost::equals(name, gaNscoreFilename) ||
			 boost::equals(name, gaSNscoreFilename))
	{
#ifdef GA_DEBUG
		std::cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name
				  << "' to '" << ((char *)value) << "'\n";
#endif
		char tmpname[64];
		memcpy(tmpname, value, strlen((char *)value) + 1);
		stats.scoreFilename(tmpname);
		status = 0;
	}
	return status;
}

// This is a pretty ugly little hack to make doubles/floats work transparently.
int GAGeneticAlgorithm::set(const std::string &name, double v)
{
	int status = 1;
	for (const auto &p : params)
	{
		if (name == p.fullname() || name == p.shrtname())
		{
			if (p.type() == ParType::FLOAT)
			{
				auto fval = static_cast<float>(v);
				status = setptr(name, (void *)&fval);
			}
			else
			{
				status = setptr(name, (void *)&v);
			}
		}
	}
	return status;
}

int GAGeneticAlgorithm::get(const char *name, void *value) const
{
	int status = 1;

	if (strcmp(name, gaNseed) == 0 || strcmp(name, gaSNseed) == 0)
	{
		*(static_cast<int *>(value)) = d_seed;
		status = 0;
	}
	else if (strcmp(name, gaNnBestGenomes) == 0 ||
			 strcmp(name, gaSNnBestGenomes) == 0)
	{
		*(static_cast<int *>(value)) = stats.nBestGenomes();
		status = 0;
	}
	else if (strcmp(name, gaNpopulationSize) == 0 ||
			 strcmp(name, gaSNpopulationSize) == 0)
	{
		*(static_cast<int *>(value)) = pop->size();
		status = 0;
	}
	else if (strcmp(name, gaNminimaxi) == 0 || strcmp(name, gaSNminimaxi) == 0)
	{
		*(static_cast<int *>(value)) = minmax;
		status = 0;
	}
	else if (strcmp(name, gaNnGenerations) == 0 ||
			 strcmp(name, gaSNnGenerations) == 0)
	{
		*(static_cast<int *>(value)) = ngen;
		status = 0;
	}
	else if (strcmp(name, gaNpConvergence) == 0 ||
			 strcmp(name, gaSNpConvergence) == 0)
	{
		*(static_cast<float *>(value)) = pconv;
		status = 0;
	}
	else if (strcmp(name, gaNnConvergence) == 0 ||
			 strcmp(name, gaSNnConvergence) == 0)
	{
		*(static_cast<int *>(value)) = nconv;
		status = 0;
	}
	else if (strcmp(name, gaNpCrossover) == 0 ||
			 strcmp(name, gaSNpCrossover) == 0)
	{
		*(static_cast<float *>(value)) = pcross;
		status = 0;
	}
	else if (strcmp(name, gaNpMutation) == 0 ||
			 strcmp(name, gaSNpMutation) == 0)
	{
		*(static_cast<float *>(value)) = pmut;
		status = 0;
	}
	else if (strcmp(name, gaNscoreFrequency) == 0 ||
			 strcmp(name, gaSNscoreFrequency) == 0)
	{
		*(static_cast<int *>(value)) = stats.scoreFrequency();
		status = 0;
	}
	else if (strcmp(name, gaNflushFrequency) == 0 ||
			 strcmp(name, gaSNflushFrequency) == 0)
	{
		*(static_cast<int *>(value)) = stats.flushFrequency();
		status = 0;
	}
	else if (strcmp(name, gaNrecordDiversity) == 0 ||
			 strcmp(name, gaSNrecordDiversity) == 0)
	{
		*(static_cast<int *>(value)) = static_cast<int>(stats.recordDiversity());
		status = 0;
	}
	else if (strcmp(name, gaNselectScores) == 0 ||
			 strcmp(name, gaSNselectScores) == 0)
	{
		*(static_cast<int *>(value)) = stats.selectScores();
		status = 0;
	}
	else if (strcmp(name, gaNscoreFilename) == 0 ||
			 strcmp(name, gaSNscoreFilename) == 0)
	{
		*(static_cast<const char **>(value)) = stats.scoreFilename().c_str();
		status = 0;
	}
	return status;
}

void GAGeneticAlgorithm::objectiveFunction(GAGenome::Evaluator f)
{
	for (int i = 0; i < pop->size(); i++)
	{
		pop->individual(i).evaluator(f);
	}
}

void GAGeneticAlgorithm::objectiveData(const GAEvalData &v)
{
	for (int i = 0; i < pop->size(); i++)
	{
		pop->individual(i).evalData(v);
	}
}

const GAPopulation &GAGeneticAlgorithm::population(const GAPopulation &p)
{
	if (p.size() < 1)
	{
		GAErr(GA_LOC, className(), "population", GAError::NoIndividuals);
		return *pop;
	}

	pop->copy(p);
	pop->geneticAlgorithm(*this);

	return *pop;
}

int GAGeneticAlgorithm::populationSize(unsigned int value)
{
	unsigned int ps = value;
	params.set(gaNpopulationSize, value);
	return pop->size(ps);
}

int GAGeneticAlgorithm::minimaxi(int m)
{
	if (m == MINIMIZE)
	{
		pop->order(GAPopulation::LOW_IS_BEST);
	}
	else
	{
		pop->order(GAPopulation::HIGH_IS_BEST);
	}
	params.set(gaNminimaxi, m);
	minmax = (m == MINIMIZE ? MINIMIZE : MAXIMIZE);
	return minmax;
}
