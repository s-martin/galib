// $Header$
/* ----------------------------------------------------------------------------
  config.h
  mbwall 27jun95
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
						  all rights reserved
  Copyright (c) 1998-2005 Matthew Wall
						  all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_config_h_
#define _ga_config_h_

#include <boost/predef.h>

/* ----------------------------------------------------------------------------
PREPROCESSOR DIRECTIVES

  Here are the preprocessor directives that the library understands.

  Some of these are already set up for the OSes with which I am familiar.  See
below for the pre-defined sets.  If you come up with a compiler/platform
configuration that is not listed here, please send it to me so that i can
incorporate it into the code base.

  GALIB_USE_PID

					 Define this if the system has a getpid function that
					 returns something sane and useful.

  GALIB_USE_BORLAND_INST

					  For compilers that use the Borland instantiation model.
					  These compilers expect all of the template code to be
			  in the header file.  The Cfront model, on the other
					  hand, expects source files with names similar to the
			  header files, but all of the template code does not
			  need to be in the header files.

			  When you define this flag, the source file that
			  corresponds to the header file is explicitly included
			  at the end of the header file for all headers that
			  contain templates.

  GALIB_USE_AUTO_INST

					  For compilers that do not do automatic instantiation
					  (such as g++ version 2.6.8) you will have to force
					  instantiations.  When this flag is not defined, GAlib
			  forces an instantiation of all of the template classes
			  that it uses (such as real genome and string genome).


  USE_GALIB_AS_LIB       For windows shared libraries, one must define whether
  USE_GALIB_AS_DLL       static member data are imported or exported.  You
						 define one or the other of these, but not both.  The
			 default is USE_GALIB_AS_LIB (if you define neither).

  COMPILE_GALIB_AS_LIB   If you are compiling the dome library, define one of
  COMPILE_GLAIB_AS_DLL   these to indicate the windows exports.  The default
						 is USE_GALIB_AS_LIB (if you define neither).



  GALIB_USE_RAN1      These specify which random number function to use.  Only
  GALIB_USE_RAN2      one of these may be specified.  You may have to tweak
  GALIB_USE_RAN3      random.h a bit as well (these functions are not defined
  GALIB_USE_RAND      the same way on each platform).  For best results, use
  GALIB_USE_RANDOM    ran2 or ran3 (performance is slightly slower than most
  GALIB_USE_RAND48    system RNGs, but you'll get better results).

					  If you want to use another random number generator you
					  must hack random.h directly (see the comments in that
					  file).

  GALIB_BITBASE       The built-in type to use for bit conversions.  This
					  should be set to the type of the largest integer that
					  your system supports.  If you have long long int then
					  use it.  If you don't plan to use more than 16 or 32
					  bits to represent your binary-to-decimal mappings then
					  you can use something smaller (long int for example).
					  If you do not set this, GAlib will automatically use
					  the size of a long int.  The bitbase determines the
			  maximum number of bits you can use to represent a
			  decimal number in the binary-to-decimal genomes.

  GALIB_BITS_IN_WORD  How many bits are in a word?  For many systems, a word is
					  a char and is 8 bits long.

---------------------------------------------------------------------------- */

// This is a bare-bones os-cpu-compiler detection with no dependencies on any
// other header files or macros.  We try to detect everything based on what we
// will get by default from the compilers.  The result of this is three macros
//   GALIB_OS
//   GALIB_CPU
//   GALIB_COMPILER

// determine the operating system
#if BOOST_OS_LINUX
#define GALIB_OS "linux"
#elif BOOST_OS_IRIX
#define GALIB_OS "irix"
#elif BOOST_OS_WINDOWS
#define GALIB_OS "win32"
#elif BOOST_OS_SOLARIS
#define GALIB_OS "solaris"
#elif BOOST_OS_MACOS
#define GALIB_OS "macos"
#elif BOOST_OS_HPUX
#define GALIB_OS "hpux"
#elif BOOST_OS_AIX
#define GALIB_OS "aix"
#else
#define GALIB_OS "unknown"
#endif

// determine the cpu
#if BOOST_ARCH_X86_32
#define GALIB_CPU "i386"
#elif BOOST_ARCH_X86_64
#define GALIB_CPU "amd64"
#elif BOOST_ARCH_PPC
#define GALIB_CPU "ppc"
#else
#define GALIB_CPU "unknown"
#endif

// determine the compiler
#if BOOST_COMP_GNUC
#define GALIB_COMPILER "gcc"
#elif BOOST_COMP_CLANG
#define GALIB_COMPILER "clang"
#elif BOOST_COMP_BORLAND
#define GALIB_COMPILER "bcc"
#elif BOOST_COMP_MSVC
#define GALIB_COMPILER "msvc"
#else
#define GALIB_COMPILER "unknown"
#endif

// ----------------------------------------------------------------------------
// Here are the defines needed for some of the compilers/OSes on which I have
// been able to test the GA library.  You may have to remove and/or modify
// these to get things to work on your system.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// borland c++ compiler
//
// You may or may not need the BORLAND_INST flag defined when you use a borland
// compiler.  I did not need it when I compiled using version 4.0, but I did
// need it when I compiled with an earlier version (I think it was 3.x but I
// do not remember for certain).
//   Note that the default random number generator when using a borland (or
// any PC compiler, for that matter) is the basic system's RNG.
// I did this because of the hassles of 16- vs 32-bit DOS/Windows rubbish.  If
// you want a better RNG, you can use the others in GAlib, but you'll have to
// do a bit of checking to make sure it works with your DOS/Windows config.
// All of the RNGs work fine under all of the 32-bit OSes I've tried, but they
// don't do so well in a 16-bit OS.
//  Use the randtest example to check GAlib's RNG after you compile everything.
#if BOOST_COMP_BORLAND
//#define GALIB_USE_RAND	// comment this if you're using a 32-bit OS
#define GALIB_USE_BORLAND_INST
//#define GALIB_USE_PID

//#pragma warning (disable : 8027)    // switch statements are not inlined
//#pragma warning (disable : 8004)    // unused variable warnings are lame

// ----------------------------------------------------------------------------
// MicroSoft's Visual C++ programming environment.
#elif BOOST_COMP_MSVC
// visual studio uses the borland model of template instantiation.
#define GALIB_USE_BORLAND_INST
// let visual studio do its own instantations, so by default do not force them.
#define GALIB_USE_AUTO_INST
// we default to using std things in the std namespace, but depending on the
// version of vcpp you are using and depending on the libraries with which you
// will use GAlib, you might have to turn this one off.
// use the pid only on winnt and derivatives.  win95/98/ME do not have it.
// this requires unistd.h, which you may or may not have (depending on the
// way that you installed the compiler).
//#define GALIB_USE_PID

// there are many warnings from vcpp, many of which we can safely ignore.
//#pragma warning (disable : 4244)    // int-to-float warnings
//#pragma warning(disable : 4305) // double-to-float warnings
//#pragma warning(disable : 4355) // allow us to use this in constructors
//#pragma warning (disable : 4250)    // dominated multiple inherits

// ----------------------------------------------------------------------------
// GNU compiler
//
// there are some subtle differences in the way the gcc compiler handles
// templates from one version to another.  the latest versions are much more
// strictly adherant to the c++ standards, although you can relax that with
// the permissive option.  we try to build this library without the use of the
// permissive option.
#elif BOOST_COMP_GNUC || BOOST_COMP_CLANG
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_PID

// ----------------------------------------------------------------------------
// This is an unknown/untested platform and/or compiler.  The defaults below
// might work for you, but then again, they might not.  You may have to adjust
// the values of the macros until everything works properly.  Comment out the
// #error directive to allow things to compile properly.
#else
#error Unknown/untested compiler/operating system!  Check these settings!

#define GALIB_USE_BORLAND_INST
#define GALIB_USE_AUTO_INST
#define GALIB_USE_PID
#endif

#include <cassert>

// If no RNG has been selected, use the ran2 generator by default
#if !defined(GALIB_USE_RAND) && !defined(GALIB_USE_RANDOM) &&                  \
	!defined(GALIB_USE_RAND48) && !defined(GALIB_USE_RAN2) &&                  \
	!defined(GALIB_USE_RAN3)
#define GALIB_USE_RAN2
#endif

// This defines how many bits are in a single word on your system.  Most
// systems have a word length of 8 bits.
constexpr int GALIB_BITS_IN_WORD = 8;

// Use this to set the maximum number of bits that can be used in binary-to-
// decimal conversions.  You should make this type the largest integer type
// that your system supports.
using GALIB_BITBASE = long int;

// If the system/compiler understands C++ casts, then we use them.  Otherwise
// we default to the C-style casts.  The macros make explicit the fact that we
// are doing casts.
#define DYN_CAST(type, x) (dynamic_cast<type>(x))
#define CON_CAST(type, x) (const_cast<type>(x))
#define STA_CAST(type, x) (static_cast<type>(x))
#define REI_CAST(type, x) (reinterpret_cast<type>(x))

// Windows is brain-dead about how to export things, so we do this to keep the
// code (somewhat) cleaner but still accomodate windows' stupidity.
#if defined(COMPILE_GALIB_AS_DLL)
#define GA_DLLDECL __declspec(dllexport)
#elif defined(USE_GALIB_AS_DLL)
#define GA_DLLDECL __declspec(dllimport)
#else
#define GA_DLLDECL
#endif

/* ----------------------------------------------------------------------------
DEFAULT OPERATORS

  These directives determine which operators will be used by default for each
of the objects in GAlib.
---------------------------------------------------------------------------- */
// scaling schemes
#define USE_LINEAR_SCALING 1
#define USE_SIGMA_TRUNC_SCALING 1
#define USE_POWER_LAW_SCALING 1
#define USE_SHARING 1

// selection schemes
#define USE_RANK_SELECTOR 1
#define USE_ROULETTE_SELECTOR 1
#define USE_TOURNAMENT_SELECTOR 1
#define USE_DS_SELECTOR 1
#define USE_SRS_SELECTOR 1
#define USE_UNIFORM_SELECTOR 1

// These are the compiled-in defaults for various genomes and GA objects
#define DEFAULT_SCALING GALinearScaling
#define DEFAULT_SELECTOR GARouletteWheelSelector
#define DEFAULT_TERMINATOR TerminateUponGeneration

#define DEFAULT_1DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_1DBINSTR_MUTATOR FlipMutator
#define DEFAULT_1DBINSTR_COMPARATOR BitComparator
#define DEFAULT_1DBINSTR_CROSSOVER OnePointCrossover
#define DEFAULT_2DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_2DBINSTR_MUTATOR FlipMutator
#define DEFAULT_2DBINSTR_COMPARATOR BitComparator
#define DEFAULT_2DBINSTR_CROSSOVER OnePointCrossover
#define DEFAULT_3DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_3DBINSTR_MUTATOR FlipMutator
#define DEFAULT_3DBINSTR_COMPARATOR BitComparator
#define DEFAULT_3DBINSTR_CROSSOVER OnePointCrossover

#define DEFAULT_BIN2DEC_ENCODER GABinaryEncode
#define DEFAULT_BIN2DEC_DECODER GABinaryDecode
#define DEFAULT_BIN2DEC_COMPARATOR BitComparator

#define DEFAULT_1DARRAY_INITIALIZER NoInitializer
#define DEFAULT_1DARRAY_MUTATOR SwapMutator
#define DEFAULT_1DARRAY_COMPARATOR ElementComparator
#define DEFAULT_1DARRAY_CROSSOVER OnePointCrossover
#define DEFAULT_2DARRAY_INITIALIZER NoInitializer
#define DEFAULT_2DARRAY_MUTATOR SwapMutator
#define DEFAULT_2DARRAY_COMPARATOR ElementComparator
#define DEFAULT_2DARRAY_CROSSOVER OnePointCrossover
#define DEFAULT_3DARRAY_INITIALIZER NoInitializer
#define DEFAULT_3DARRAY_MUTATOR SwapMutator
#define DEFAULT_3DARRAY_COMPARATOR ElementComparator
#define DEFAULT_3DARRAY_CROSSOVER OnePointCrossover

#define DEFAULT_1DARRAY_ALLELE_INITIALIZER UniformInitializer
#define DEFAULT_1DARRAY_ALLELE_MUTATOR FlipMutator
#define DEFAULT_1DARRAY_ALLELE_COMPARATOR ElementComparator
#define DEFAULT_1DARRAY_ALLELE_CROSSOVER OnePointCrossover
#define DEFAULT_2DARRAY_ALLELE_INITIALIZER UniformInitializer
#define DEFAULT_2DARRAY_ALLELE_MUTATOR FlipMutator
#define DEFAULT_2DARRAY_ALLELE_COMPARATOR ElementComparator
#define DEFAULT_2DARRAY_ALLELE_CROSSOVER OnePointCrossover
#define DEFAULT_3DARRAY_ALLELE_INITIALIZER UniformInitializer
#define DEFAULT_3DARRAY_ALLELE_MUTATOR FlipMutator
#define DEFAULT_3DARRAY_ALLELE_COMPARATOR ElementComparator
#define DEFAULT_3DARRAY_ALLELE_CROSSOVER OnePointCrossover

#define DEFAULT_STRING_INITIALIZER UniformInitializer
#define DEFAULT_STRING_MUTATOR FlipMutator
#define DEFAULT_STRING_COMPARATOR ElementComparator
#define DEFAULT_STRING_CROSSOVER UniformCrossover

#define DEFAULT_REAL_INITIALIZER UniformInitializer
#define DEFAULT_REAL_MUTATOR GARealGaussianMutator
#define DEFAULT_REAL_COMPARATOR ElementComparator
#define DEFAULT_REAL_CROSSOVER UniformCrossover

#define DEFAULT_TREE_INITIALIZER NoInitializer
#define DEFAULT_TREE_MUTATOR SwapSubtreeMutator
#define DEFAULT_TREE_COMPARATOR TopologyComparator
#define DEFAULT_TREE_CROSSOVER OnePointCrossover

#define DEFAULT_LIST_INITIALIZER NoInitializer
#define DEFAULT_LIST_MUTATOR SwapMutator
#define DEFAULT_LIST_COMPARATOR NodeComparator
#define DEFAULT_LIST_CROSSOVER OnePointCrossover

#endif
