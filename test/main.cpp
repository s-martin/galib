#define BOOST_TEST_MODULE GALibTest

// On Windows we link against the pre-built (DLL/static) Boost.Test library, so we
// only need the lightweight, non-included header here. On other platforms we use
// the header-only ("included") variant so the test binary doesn't depend on a
// separately built/linked Boost.Test library.
#ifdef _WIN32
#include <boost/test/unit_test.hpp>
#else
#include <boost/test/included/unit_test.hpp>
#endif
