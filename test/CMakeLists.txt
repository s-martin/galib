
add_executable("${PROJECT_NAME}Test" 
        "main.cpp"
        "GASimpleGATest.cpp"
        "GASStateGATest.cpp"
        "GAExamplesTest.cpp"
		"GAParameterTest.cpp")

target_include_directories("${PROJECT_NAME}Test" PUBLIC "../ga")
target_include_directories(${PROJECT_NAME}Test PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}Test" PUBLIC GAlib Boost::unit_test_framework Boost::program_options)

add_test(NAME "${PROJECT_NAME}Test" COMMAND "${PROJECT_NAME}Test")
