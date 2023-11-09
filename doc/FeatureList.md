# Feature List for GAlib

> Original author Matthew Wall. All rights reserved.<br>
> Copyright (c) 1995-1996 Massachusetts Institute of Technology<br>
> Copyright (c) 1996-1999 Matthew Wall

## General Features

- Many examples are included illustrating the use of various GAlib features, class derivations, parallelization, deterministic crowding, travelling salesman, DeJong, and Royal Road problems.
- The library has been used on various Windows and Linux configurations. GAlib compiles without warnings on most major compilers.
- Templates are used in some genome classes, but GAlib can be used without templates if your compiler does not understand them.
- Four random number generators are included with the library.  You can select the one most appropriate for your system, or use your own.

## Algorithms, Parameters, and Statistics

- GAlib can be used with PVM (parallel virtual machine) to evolve populations and/or individuals in parallel on multiple CPUs.
- Genetic algorithm parameters can be configured from file, command-line, and/or code.
- Overlapping (steady-state GA) and non-overlapping (simple GA) populations are supported.  You can also specify the amount of overlap (% replacement).  The distribution includes examples of other derived genetic algorithms such as a genetic algorithm with sub-populations and another that uses deterministic crowding.
- New genetic algorithms can be quickly tested by deriving from the base genetic algorithm classes in the library.  In many cases you need only overide one virtual function.
- Built-in termination methods include convergence and number-of-generations.  The termination method can be customized for any existing genetic algorithm class or for new classes you derive.
- Speciation can be done with either DeJong-style crowding (using a replacement strategy) or Goldberg-style sharing (using fitness scaling).
- Elitism is optional for non-overlapping genetic algorithms.
- Built-in replacement strategies (for overlapping populations) include replace parent, replace random, replace worst.  The replacement operator can be customized.
- Built-in selection methods include rank, roulette wheel, tournament, stochastic remainder sampling, stochastic uniform sampling, and deterministic sampling.  The selection operator can be customized.
- "on-line" and "off-line" statistics are recorded as well as max, min, mean, standard deviation, and diversity.  You can specify which statistics should be recorded and how often they should be flushed to file.

## Genomes and Operators

- Chromosomes can be built from *any* C++ data type.  You can use the types built-in to the library (bit-string, array, list, tree) or derive a chromosome based on your own objects.
- Built-in chromosome types include real number arrays, list, tree, 1D, 2D, and 3D arrays, 1D, 2D, and 3D binary string.  The binary strings, strings, and arrays can be variable length.  The lists and trees can contain any object in their nodes.  The array can contain any object in each element.
- All chromosome initialization, mutation, crossover, and comparison methods can be customized.
- Built-in initialization operators include uniform random, order-based random, allele-based random, and initialize-to-zero.
- Built-in mutation operators include random flip, random swap, Gaussian, destructive, swap subtree, swap node.
- Built-in crossover operators include arithmetic, blend, partial match, ordered, cycle, single point, two point, even, odd, uniform, node- and subtree-single point.  Edge recombination is included in the examples.
- Dominance and Diploidy are not explicitly built in to the library, but any of the genome classes in the library can easily be extended to become diploid chromosomes.

## Objective Function

- Objective functions can be population- or individual-based.
- If the built-in genomes adequately represent your problem, a user-specified objective function is the only problem-specific code that *must* be written.
