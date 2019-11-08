set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("${PROJECT_NAME}Test" 
        "main.cpp"
		"GAArrayTest.cpp"
		"GAMaskTest.cpp"
		"GAListTest.hpp
        "GAExamplesTest.cpp")

target_include_directories("${PROJECT_NAME}Test" PUBLIC "../ga")
target_include_directories("${PROJECT_NAME}Test" PUBLIC "../examples")
target_include_directories(${PROJECT_NAME}Test PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}Test" PUBLIC GAlib Boost::unit_test_framework)

add_test(NAME "${PROJECT_NAME}Test" COMMAND "${PROJECT_NAME}Test")
