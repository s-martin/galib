#pragma once

#include <GA2DBinStrGenome.h>
#include <GASStateGA.h>

#include <fstream>
#include <iostream>

// Declarations only; definitions moved to examples/ex7.C
float objectiveEx7(GAGenome& c);
float objective(GAGenome& c);
GAStatistics example7(GAParameterList params, const std::string &datafile);
