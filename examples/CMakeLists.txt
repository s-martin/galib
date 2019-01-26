include_directories(${CMAKE_SOURCE_DIR}/ga)
set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)

add_executable(example1 ex1.C)
target_compile_features(example1 PUBLIC cxx_std_17)
target_link_libraries(example1 PUBLIC GAlib)

add_executable(example2 ex2.C)
target_compile_features(example2 PUBLIC cxx_std_17)
target_link_libraries(example2 PUBLIC GAlib)

add_executable(example3 ex3.C)
target_compile_features(example3 PUBLIC cxx_std_17)
target_link_libraries(example3 PUBLIC GAlib)

add_executable(example4 ex4.C)
target_compile_features(example4 PUBLIC cxx_std_17)
target_link_libraries(example4 PUBLIC GAlib)

add_executable(example5 ex5.C)
target_compile_features(example5 PUBLIC cxx_std_17)
target_link_libraries(example5 PUBLIC GAlib)

# add_executable(example6 ex6.C)
# target_compile_features(example6 PUBLIC cxx_std_17)
# target_link_libraries(example6 PUBLIC GAlib)

add_executable(example7 ex7.C)
target_compile_features(example7 PUBLIC cxx_std_17)
target_link_libraries(example7 PUBLIC GAlib)

# add_executable(example8 ex8.C)
# target_compile_features(example8 PUBLIC cxx_std_17)
# target_link_libraries(example8 PUBLIC GAlib)

# add_executable(pvmpop 
#                 ./pvmpop/genome.C 
#                 ./pvmpop/genome.h 
#                 ./pvmpop/master.C
#                 ./pvmpop/PVMDemeGA.C 
#                 ./pvmpop/PVMDemeGA.h
#                 ./pvmpop/slave.C)
# target_compile_features(pvmpop PUBLIC cxx_std_17)
# target_link_libraries(pvmpop PUBLIC GAlib)
