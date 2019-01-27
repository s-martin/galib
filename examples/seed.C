/* ----------------------------------------------------------------------------
  seed.C
  mbwall 16nov98
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cstdlib>

#include <ga.h>
 

#include <iostream>

 
 
 

float objective(GAGenome &);

int
main(int argc, char *argv[]) {
  std::cout << "Random Seed Test\n\n";
  std::cout << "This program does three runs of a genetic algorithm, with the \n";
  std::cout << "random seed resetting between each run.  Each of the three runs \n";
  std::cout << "should be identical\n\n";
  std::cout.flush();

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNnGenerations, 100);
  params.set(gaNflushFrequency, 5);
  params.set(gaNpMutation, 0.001);
  params.set(gaNpCrossover, 0.8);
  params.parse(argc, argv, false);

  int i,j;
  char filename[128] = "smiley.txt";
  unsigned int seed=0;

  for(i=1; i<argc; i++){
    if(strcmp("file", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
         std::cerr << argv[0] << ": the file option needs a filename.\n";
        exit(1);
      }
      else{
        sprintf(filename, argv[i]);
        continue;
      }
    }
    else if(strcmp("seed", argv[i]) == 0){
      if(++i >= argc){
         std::cerr << argv[0] << ": the seed option needs a filename.\n";
        exit(1);
      }
      else {
	seed = atoi(argv[i]);
	continue;
      }
    }
    else {
       std::cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
       std::cerr << "valid arguments include standard GAlib arguments plus:\n";
       std::cerr << "  f\tfilename from which to read (" << filename << ")\n";
       std::cerr << "\n";
      exit(1);
    }
  }

  const int n=5;

  std::cout << n << " random numbers\n";
  GAResetRNG(seed);
  for(i=0; i<n; i++)
    std::cout << " " << GARandomFloat();
  std::cout << "\n";

  std::cout << n << " random numbers\n";
  GAResetRNG(seed);
  for(i=0; i<n; i++)
    std::cout << " " << GARandomFloat();
  std::cout << "\n";

  std::cout << n << " random numbers\n";
  GAResetRNG(seed);
  for(i=0; i<n; i++)
    std::cout << " " << GARandomFloat();
  std::cout << "\n";
  std::cout.flush();

   std::ifstream inStream(filename);
  if(!inStream){
     std::cerr << "Cannot open " << filename << " for input.\n";
    exit(1);
  }

  int height, width;
  inStream >> height >> width;

  short **target = new short*[width];
  for(i=0; i<width; i++)
    target[i] = new short[height];

  for(j=0; j<height; j++)
    for(i=0; i<width; i++)
      inStream >> target[i][j];

  inStream.close();

  GA2DBinaryStringGenome genome(width, height, objective, (void *)target);
  GASimpleGA ga(genome);
  ga.parameters(params);

  // first run

  GAResetRNG(seed);
  genome.initialize();
  std::cout << genome << "\n";
  ga.set(gaNscoreFilename, "bog1.dat");
  ga.evolve();

  genome = ga.statistics().bestIndividual();
  std::cout << "run 1:  the random seed is: " << GAGetRandomSeed() << "\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      std::cout << (genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

  // second run

  GAResetRNG(seed);
  genome.initialize();
  std::cout << genome << "\n";
  ga.set(gaNscoreFilename, "bog2.dat");
  ga.evolve();

  genome = ga.statistics().bestIndividual();
  std::cout << "run 2:  the random seed is: " << GAGetRandomSeed() << "\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      std::cout << (genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

  // third run

  GAResetRNG(seed);
  genome.initialize();
  std::cout << genome << "\n";
  ga.set(gaNscoreFilename, "bog3.dat");
  ga.evolve();

  genome = ga.statistics().bestIndividual();
  std::cout << "run 3:  the random seed is: " << GAGetRandomSeed() << "\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      std::cout << (genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    std::cout << "\n";
  }
  std::cout << "\n"; std::cout.flush();

  for(i=0; i<width; i++)
    delete target[i];
  delete [] target;

  return 0;
}
 

float
objective(GAGenome & c) {
  GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)c;
  short **pattern = (short **)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; j<genome.height(); j++)
      value += (float)(genome.gene(i,j) == pattern[i][j]);
  return(value);
}
