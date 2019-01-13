include_directories(${CMAKE_SOURCE_DIR}/ga)

add_executable(example1 ex1.C)
target_compile_features(example1 PUBLIC cxx_std_17)
set_target_properties(example1 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example1 PUBLIC GAlib)

add_executable(example2 ex2.C)
target_compile_features(example2 PUBLIC cxx_std_17)
set_target_properties(example2 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example2 PUBLIC GAlib)

add_executable(example3 ex3.C)
target_compile_features(example3 PUBLIC cxx_std_17)
set_target_properties(example3 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example3 PUBLIC GAlib)

add_executable(example4 ex4.C)
target_compile_features(example4 PUBLIC cxx_std_17)
set_target_properties(example4 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example4 PUBLIC GAlib)

add_executable(example5 ex5.C)
target_compile_features(example5 PUBLIC cxx_std_17)
set_target_properties(example5 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example5 PUBLIC GAlib)
