#pragma once

#include <GASStateGA.h>
#include <GATreeGenome.hpp>

#include <iostream>

// Declarations only; definitions moved to examples/ex6.C
float objectiveEx6(GAGenome &c);
void TreeInitializer(GAGenome &c);

void WriteNode(std::ostream &os, GANode<int> *n);

GATreeGenome<int> ex6(GAParameterList params, unsigned int seed);
float objective(GAGenome &c);
GAStatistics example6(GAParameterList params, unsigned int seed);
