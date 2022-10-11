#pragma once

#include <GA2DBinStrGenome.h>
#include <GASimpleGA.h>

#include <fstream>
#include <iostream>


// For the objective function we compare the contents of the genome with the
// target.  If a bit is set in the genome and it is also set in the target,
// then we add 1 to the score.  If the bits do not match, we don't do anything.
float objectiveEx3(GAGenome& c)
{
	auto& genome = (GA2DBinaryStringGenome&)c;
	auto** pattern = (short**)c.userData();

	float value = 0.0;
	for (int i = 0; i < genome.width(); i++)
		for (int j = 0; j < genome.height(); j++)
			value += (float)(genome.gene(i, j) == pattern[i][j]);
	return (value);
}

GASimpleGA ex3(const std::shared_ptr<GAParameterList>& params, const std::string& filename)
{
	// Read in the pattern from the specified file.  File format is pretty
	// simple: two integers that give the height then width of the matrix, then
	// the matrix of 1's and 0's (with whitespace inbetween).
	std::ifstream inStream(filename);
	if (!inStream)
	{
		std::cerr << "Cannot open " << filename << " for input.\n";
		exit(1);
	}

	int height, width;
	inStream >> height >> width;

	auto** target = new short* [width];
	for (int i = 0; i < width; i++)
		target[i] = new short[height];

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			inStream >> target[i][j];

	inStream.close();

	// Print out the pattern to be sure we got the right one.

	std::cout << "input pattern:\n";
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
			std::cout << (target[i][j] == 1 ? '*' : ' ') << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout.flush();

	// Now create the GA and run it.

	GA2DBinaryStringGenome genome(width, height, objectiveEx3, (void*)target);
	GASimpleGA ga(genome, params);
	ga.evolve();

	std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";
	genome = ga.statistics().bestIndividual();
	std::cout << "the ga generated:\n";
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
			std::cout << (genome.gene(i, j) == 1 ? '*' : ' ') << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout.flush();

	for (int i = 0; i < width; i++)
		delete target[i];
	delete[] target;

	return ga;
}
