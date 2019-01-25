cmake_minimum_required(VERSION 3.1...3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.13)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.13)
endif()

project(GAlib VERSION 3.0
                  DESCRIPTION "GAlib using modern C++"
                  LANGUAGES CXX)

if(MSVC)
    add_definitions("/W4 /D_CRT_SECURE_NO_WARNINGS /permissive-")
else()
    add_definitions("-Wall -Wextra")
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65 REQUIRED COMPONENTS filesystem unit_test_framework)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories( ${Boost_INCLUDE_DIRS})
                  
# This is needed if your Boost version is newer than your CMake version
# or if you have an old version of CMake (<3.5)
#if(NOT TARGET Boost::filesystem)
#    add_library(Boost::filesystem IMPORTED INTERFACE)
#    set_property(TARGET Boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
#    set_property(TARGET Boost::filesystem PROPERTY INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
#endif()

enable_testing()
include(CTest)
                  
add_subdirectory(ga)
add_subdirectory(examples)
add_subdirectory(test)

