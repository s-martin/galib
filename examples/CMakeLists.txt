
add_executable(example1 ex1.C)
target_include_directories(example1 PUBLIC ${CMAKE_SOURCE_DIR}/ga)
target_compile_features(example1 PUBLIC cxx_std_17)
set_target_properties(example1 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example1 PUBLIC GAlib-NG)

add_executable(example2 ex2.C)
target_include_directories(example2 PUBLIC ${CMAKE_SOURCE_DIR}/ga)
target_compile_features(example2 PUBLIC cxx_std_17)
set_target_properties(example2 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example2 PUBLIC GAlib-NG)

add_executable(example3 ex3.C)
target_include_directories(example3 PUBLIC ${CMAKE_SOURCE_DIR}/ga)
target_compile_features(example3 PUBLIC cxx_std_17)
set_target_properties(example3 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example3 PUBLIC GAlib-NG)

add_executable(example4 ex4.C)
target_include_directories(example4 PUBLIC ${CMAKE_SOURCE_DIR}/ga)
target_compile_features(example4 PUBLIC cxx_std_17)
set_target_properties(example4 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(example4 PUBLIC GAlib-NG)