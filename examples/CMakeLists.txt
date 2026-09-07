message(STATUS "Building examples")

include_directories(${PROJECT_SOURCE_DIR}/ga)
include_directories(${Boost_INCLUDE_DIRS})
set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_galib_example target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PUBLIC GAlib)
endfunction()

# TODO ex12 and ex15-ex27 remain excluded from CI until their modernized headers/sources are fully reconciled.
add_galib_example(example1 ex1.C)
add_galib_example(example2 ex2.C)
add_galib_example(example3 ex3.C)
add_galib_example(example4 ex4.C)
add_galib_example(example5 ex5.C)
add_galib_example(example6 ex6.C)
add_galib_example(example7 ex7.C)
add_galib_example(example8 ex8.C)
add_galib_example(example9 ex9.C)
add_galib_example(example10 ex10.C)
add_galib_example(example11 ex11.C)
add_galib_example(example13 ex13.C)
add_galib_example(example14 ex14.C)

add_galib_example(randtest randtest.C)

if(X11_FOUND)
    message(STATUS "Building graphic examples")
    add_executable(gaview
                        ./graphic/gaview.C )
    target_link_libraries(gaview GAlib Xaw ${X11_Xt_LIB} ${X11_LIBRARIES})
    target_include_directories(gaview PUBLIC ${X11_INCLUDE_DIR})

    add_executable(tspview
                        ./graphic/tspview.C)
    target_link_libraries(tspview GAlib Xaw ${X11_Xt_LIB} ${X11_LIBRARIES})
    target_include_directories(tspview PUBLIC ${X11_INCLUDE_DIR})
endif()

if(BUILD_PVM)
    find_library(PVM_LIB pvm3)

    if(PVM_LIB)
        message(STATUS "Building PVM examples")
        add_executable(pvmpop_master
                ./pvmpop/genome.C
                ./pvmpop/genome.h
                ./pvmpop/master.C
                ./pvmpop/PVMDemeGA.C
                ./pvmpop/PVMDemeGA.h)
        target_link_libraries(pvmpop_master PUBLIC GAlib ${PVM_LIB})
        target_include_directories(pvmpop_master PUBLIC "../../ga")

        add_executable(pvmpop_slave
                ./pvmpop/genome.C
                ./pvmpop/genome.h
                ./pvmpop/slave.C
                ./pvmpop/PVMDemeGA.C
                ./pvmpop/PVMDemeGA.h)
        target_link_libraries(pvmpop_slave PUBLIC GAlib ${PVM_LIB})
        target_include_directories(pvmpop_slave PUBLIC "../../ga")

        add_executable(pvmind_master
                ./pvmind/genome.C
                ./pvmind/genome.h
                ./pvmind/master.C)
        target_link_libraries(pvmind_master PUBLIC GAlib ${PVM_LIB})
        target_include_directories(pvmind_master PUBLIC "../../ga")

        add_executable(pvmind_slave
                ./pvmind/genome.C
                ./pvmind/genome.h
                ./pvmind/slave.C)
        target_link_libraries(pvmind_slave PUBLIC GAlib ${PVM_LIB})
        target_link_directories(pvmind_slave PUBLIC "../../ga")
    endif()
endif()
