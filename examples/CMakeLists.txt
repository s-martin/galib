message(STATUS "Building examples")

include_directories(${PROJECT_SOURCE_DIR}/ga)
include_directories(${Boost_INCLUDE_DIRS})
set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example1 ex1.C)
target_link_libraries(example1 PUBLIC GAlib)

add_executable(example2 ex2.C)
target_link_libraries(example2 PUBLIC GAlib)

add_executable(example3 ex3.C)
target_link_libraries(example3 PUBLIC GAlib)

add_executable(example4 ex4.C)
target_link_libraries(example4 PUBLIC GAlib)

add_executable(example5 ex5.C)
target_link_libraries(example5 PUBLIC GAlib)

add_executable(example6 ex6.C)
target_link_libraries(example6 PUBLIC GAlib)

add_executable(example7 ex7.C)
target_link_libraries(example7 PUBLIC GAlib)

add_executable(example8 ex8.C)
target_link_libraries(example8 PUBLIC GAlib)

add_executable(example9 ex9.C)
target_link_libraries(example9 PUBLIC GAlib)

add_executable(example10 ex10.C)
target_link_libraries(example10 PUBLIC GAlib)

add_executable(example11 ex11.C)
target_link_libraries(example11 PUBLIC GAlib)

add_executable(example12 ex12.C)
target_link_libraries(example12 PUBLIC GAlib)

add_executable(example13 ex13.C)
target_link_libraries(example13 PUBLIC GAlib)

add_executable(example14 ex14.C)
target_link_libraries(example14 PUBLIC GAlib)

add_executable(example15 ex15.C)
target_link_libraries(example15 PUBLIC GAlib)

add_executable(example16 ex16.C)
target_link_libraries(example16 PUBLIC GAlib)

add_executable(example17 ex17.C)
target_link_libraries(example17 PUBLIC GAlib)

add_executable(example18 ex18.C)
target_link_libraries(example18 PUBLIC GAlib)

add_executable(example19 ex19.C)
target_link_libraries(example19 PUBLIC GAlib)

add_executable(example20 ex20.C)
target_link_libraries(example20 PUBLIC GAlib)

add_executable(example21 ex21.C)
target_link_libraries(example21 PUBLIC GAlib)

add_executable(example22 ex22.C)
target_link_libraries(example22 PUBLIC GAlib)

add_executable(example23 ex23.C)
target_link_libraries(example23 PUBLIC GAlib)

add_executable(example24 ex24.C)
target_link_libraries(example24 PUBLIC GAlib)

add_executable(example25 ex25.C)
target_link_libraries(example25 PUBLIC GAlib)

add_executable(example26 ex26.C)
target_link_libraries(example26 PUBLIC GAlib)

add_executable(example27 ex27.C)
target_link_libraries(example27 PUBLIC GAlib)

add_executable(randtest randtest.C)
target_link_libraries(randtest PUBLIC GAlib)

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
