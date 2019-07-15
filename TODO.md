This is a running list of things to be done to galib.

# Todo #

- make the library thread-safe
- include rules for building both shared and static libraries
- xml input and output for saving the state of genomes and evolutions
- use double rather than float
- allow run-time modification of the random number generator
- get rid of all the casts (e.g. GASelector.C) for signed/unsigned
- be consistent in use of signed/unsigned in the class apis
- use bits for the bit-based genomes so that we reduce memory use
    by a factor of 8
- enable concurrent builds on the same source tree
- make galib 64-bit clean (remove the dangerous casts)

completed:

- fix stream references in examples (includes and cout/cerr)
- properly and automatically accommodate both <iostream.h> and
    <iostream> inclusions
- rewrite gaconfig.h to use consistent naming for options and make clear which
    are defaults, e.g. USE_ANSI_HEADERS/USE_NO_ANSI_HEADERS.
