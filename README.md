# Modern GAlib: A (modernized) C++ Genetic Algorithm Library

![GitHub last commit (branch)](https://img.shields.io/github/last-commit/s-martin/galib/master)

[![CI-Linux](https://github.com/s-martin/galib/actions/workflows/ci-linux.yml/badge.svg)](https://github.com/s-martin/galib/actions/workflows/ci-linux.yml) [![CI-Windows](https://github.com/s-martin/galib/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/s-martin/galib/actions/workflows/ci-windows.yml) [![CodeQL](https://github.com/s-martin/galib/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/s-martin/galib/actions/workflows/codeql-analysis.yml)

[![Coverage Status](https://coveralls.io/repos/github/s-martin/galib/badge.svg?branch=master)](https://coveralls.io/github/s-martin/galib?branch=master)

Copyright (c) 1994-1996 MIT, 1996-2005 Matthew Wall

GAlib is a C++ library of genetic algorithm objects.  With GAlib you can add
evolutionary algorithm optimization to almost any program using any data
representation and standard or custom selection, crossover, mutation,
scaling, and termination methods.

GAlib was originally developed by Matthew Wall.

Modernized GAlib from version 3.0 on is a fork of the [original GAlib](http://lancet.mit.edu/ga) and modernized the original code using C++17 technology.  

The library requires a C++ compiler conforming to C++17. It has been tested with

- Visual Studio 2019
- GCC 8.3, 9.4 (Ubuntu)
- LLVM/Clang 10.0.0 (Ubuntu)

Graphic examples (XWindows) are available, as are
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
and the unit tests. Here is the short version of how to build and test everything:

### Windows

Using [vcpkg](https://github.com/microsoft/vcpkg) to install dependencies and [chocolatey](https://chocolatey.org) to install tools is recommended.

- Clone or download the repository

- Install dependencies: `vcpkg install boost-test boost-program-options boost-predef`

- Install coverage tools: `choco install opencppcoverage`

#### Visual Studio 2019 or later

- Open path in Visual Studio as CMake project.

#### Previous versions of Visual Studio

- `md build && cd build`

- `cmake ../ -DCMAKE_TOOLCHAIN_FILE=<path to vcpkg>/scripts/buildsystems/vcpkg.cmake`

- Open created Visual Studio solution file in `build` directory

### Linux (Ubuntu)

- `sudo apt install libboost-test-dev libboost-program-options-dev libx11-dev libxt-dev libxaw7-dev`

If you want to use code coverage:

- `sudo apt install lcov gcovr`

If you want to use Doxygen:

- `sudo apt install doxygen mscgen dia graphviz`

If you want to use PVM examples:

- `sudo apt install pvm-dev`

Configure and build:

- `mkdir build && cd build`

- `cmake ../` (if you don't want to build examples append `-DBUILD_EXAMPLES=OFF`, if you want to build PVM append `-DBUILD_PVM=ON`)

- `make`

Run unit tests:

- `make test`

Run unit tests and create coverage:

- `make GAlib_lcov`

- `make GAlib_gcov`

### Building shared libraries

To build shared libraries append `-DBUILD_SHARED_LIBS:BOOL=ON` to the above `cmake` command.

See also <https://cmake.org/cmake/help/v3.15/variable/BUILD_SHARED_LIBS.html> in the CMake docs.

### Common Errors

If that does not work, then here are the files you might have to modify:

- ga/gaconfig.h  - this contains the macros that control library options

If you still have problems, look at Installation.html in the doc directory.

## DOCUMENTATION

Doxygen API documentation is available at <https://s-martin.github.io/galib/>.

More general documentation is available at `/doc`.

### Documentation of original GAlib (until 2.4.7)

List of bugs is at <http://lancet.mit.edu/ga/Bugs.html>

#### Original Mailing Lists

There are two GAlib mailing lists: <galib@mit.edu> and <galib-announce@mit.edu>
The first list is an unmoderated list intended as a forum for galib users to
help each other.  The second is only for announcements about GAlib updates.
To subscribe, send email to <galib-request@mit.edu> (or galib-announce-request)
with the word 'subscribe' as the subject and nothing in the body of the email.
To unsubscribe, send email with the word 'unsubscribe' as the subject.
