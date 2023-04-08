cmake_minimum_required(VERSION 3.1...3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.13)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.13)
endif()

project(GAlib VERSION 3.0
                  DESCRIPTION "GAlib using modern C++"
                  LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(MSVC)
    add_definitions("/W4 /D_CRT_SECURE_NO_WARNINGS /permissive-")
else()
    add_definitions("-Wall -Wextra -fpermissive")
	add_compile_definitions(_G_NO_NRV)
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65 REQUIRED COMPONENTS unit_test_framework) # filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")
             
# This is needed if your Boost version is newer than your CMake version
# or if you have an old version of CMake (<3.5)
#if(NOT TARGET Boost::filesystem)
#    add_library(Boost::filesystem IMPORTED INTERFACE)
#    set_property(TARGET Boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
#    set_property(TARGET Boost::filesystem PROPERTY INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
#endif()

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

if(BUILD_DOC)
    find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)

    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif(BUILD_DOC)

if(NOT MSVC) 
	find_package(X11 REQUIRED)
endif()

if(NOT MSVC) 
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

enable_testing()
include(CTest)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
	APPEND_COVERAGE_COMPILER_FLAGS()
	# TODO fix not working excludes in project
	set(COVERAGE_LCOV_EXCLUDES 'examples/*' 'test/*' '/usr/*')
    SETUP_TARGET_FOR_COVERAGE_LCOV(NAME ${PROJECT_NAME}_lcov EXECUTABLE ${PROJECT_NAME}Test)
	SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME ${PROJECT_NAME}_gcov EXECUTABLE ${PROJECT_NAME}Test)
else()
	# TODO
	#add_custom_target(coverage COMMAND "opencppcoverage --working_dir ${CMAKE_CURRENT_BINARY_DIR}/test -- ${CMAKE_CURRENT_BINARY_DIR}/test/GalibTest.exe")
endif()

                  
add_subdirectory(ga)
add_subdirectory(examples)
add_subdirectory(test)

# copy test data
file(COPY examples/smiley.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples)
file(COPY examples/smiley.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
file(COPY examples/smiley.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # for lcov coverage target TODO harmonize
