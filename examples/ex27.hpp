#pragma once

#include <cmath>
#include <GASStateGA.h>
#include <GAList.hpp>
#include <GA1DArrayGenome.hpp>
#include <iostream>

using Function = float (*)(float, float);

float Function1(float, float);
float Function2(float, float);
float Function3(float, float);
float Function4(float, float);

// Globals — defined in examples/ex27.C (single definition)
extern float ai[25];
extern float bi[25];

extern int which;
extern Function obj[];
extern float minx[];
extern float maxx[];
extern float miny[];
extern float maxy[];

// Callbacks (implemented in examples/ex27.C)
float objective(GAGenome& g);
void initializer(GAGenome& g);
int mutator(GAGenome& g, float pmut);
int crossover(const GAGenome& g1, const GAGenome& g2, GAGenome* c1, GAGenome* c2);
float comparator(const GAGenome& g1, const GAGenome& g2);

GAStatistics example27(unsigned int seed, int argc, char **argv);
