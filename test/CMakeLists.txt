set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("${PROJECT_NAME}Test" 
        "main.cpp"
		"GAArrayTest.cpp"
        "GASimpleGATest.cpp"
        "GASStateGATest.cpp"
        "GAExamplesTest.cpp"
		"GAParameterTest.cpp")

target_include_directories("${PROJECT_NAME}Test" PUBLIC "../ga")
target_include_directories(${PROJECT_NAME}Test PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}Test" PUBLIC GAlib Boost::unit_test_framework Boost::program_options)

add_test(NAME "${PROJECT_NAME}Test" COMMAND "${PROJECT_NAME}Test")
