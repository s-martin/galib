# Modern GAlib: A (modernized) C++ Genetic Algorithm Library

![](https://github.com/s-martin/galib/workflows/CI-Linux/badge.svg) ![](https://github.com/s-martin/galib/workflows/CI-Windows/badge.svg) ![](https://github.com/s-martin/galib/workflows/CodeQL/badge.svg) [![Codacy Security Scan](https://github.com/s-martin/galib/actions/workflows/codacy-analysis.yml/badge.svg)](https://github.com/s-martin/galib/actions/workflows/codacy-analysis.yml)

Copyright (c) 1994-1996 MIT, 1996-2005 Matthew Wall

GAlib is a C++ library of genetic algorithm objects.  With GAlib you can add
evolutionary algorithm optimization to almost any program using any data
representation and standard or custom selection, crossover, mutation,
scaling, and termination methods.

GAlib was originally developed by Matthew Wall.

Modernized GAlib from version 3.0 on is a fork of the [original GAlib](http://lancet.mit.edu/ga) and modernized the original code using latest C++17 technology.  

The library requires a C++ compiler conforming to C++17. It has been tested with

- Visual Studio 2019
- GCC 8.3 (Ubuntu)

Graphic examples (XWindows/Motif and MS Windows) are available, as are
parallel, distributed implementations using PVM.  There are about 30 examples
that illustrate various ways to use GAlib on a variety of problems.
In addition many unit tests are available.

## WHERE TO GET IT

Modernized GAlib:

<https://github.com/s-martin/galib>

Original GAlib 2.4.7:

<http://lancet.mit.edu/ga>

<ftp://lancet.mit.edu/pub/ga/>

## COMPILATION

CMake is used for compilation. There are three things to build: the library, the examples
and the unit tests. Here is the
short version of how to build and test everything:

### Windows

Using [vcpkg](https://github.com/microsoft/vcpkg) to install dependencies is recommended.

1. Clone or download the repository

2. Install dependencies: `vcpkg install boost-test boost-program-options boost-predef`

#### Visual Studio 2019

3. Open path in Visual Studio 2019 as CMake project.

#### Previous versions of Visual Studio

3. `md build && cd build`

4. `cmake ../ -DCMAKE_TOOLCHAIN_FILE=<path to vcpkg>/scripts/buildsystems/vcpkg.cmake`

5. Open created Visual Studio solution file in `build` directory

### Linux (Ubuntu)

1. `sudo apt install libboost-test-dev libboost-program-options-dev libx11-dev libxt-dev libxaw7-dev`

If you want to use code coverage:

2. `sudo apt install lcov`

3. `sudo apt install gcovr`

Configure and build:

4. `mkdir build && cd build`

5. `cmake ../`

6. `make`

Run unit tests:

7. `make test`

Run unit tests and create coverage:

8. `make GAlib_lcov`

9. `make GAlib_gcov`

### Building shared libraries

To build shared libraries append `-DBUILD_SHARED_LIBS:BOOL=ON` to the above `cmake`command.

See also <https://cmake.org/cmake/help/v3.15/variable/BUILD_SHARED_LIBS.html> in the CMake docs.

### Common Errors

If that does not work, then here are the files you might have to modify:

- ga/gaconfig.h  - this contains the macros that control library options

If you still have problems, look at Installation.html in the doc directory.

## DOCUMENTATION and MAILING LIST

**!!! to be updated !!!**

Complete documentation in html format is available in the doc directory.  The
distribution site contains PDF and PostScript(tm) versions.

A current list of bugs is at <http://lancet.mit.edu/ga/Bugs.html>

There are two GAlib mailing lists:  galib@mit.edu and galib-announce@mit.edu
The first list is an unmoderated list intended as a forum for galib users to
help each other.  The second is only for announcements about GAlib updates.
To subscribe, send email to galib-request@mit.edu (or galib-announce-request)
with the word 'subscribe' as the subject and nothing in the body of the email.
To unsubscribe, send email with the word 'unsubscribe' as the subject.
