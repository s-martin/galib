
add_executable("${PROJECT_NAME}Test" 
        "main.cpp"
        "GASimpleGATest.cpp"
        "GASStateGATest.cpp"
        "GAExamplesTest.cpp")

target_include_directories("${PROJECT_NAME}Test" PUBLIC "../ga")
target_include_directories(${PROJECT_NAME}Test PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}Test" PUBLIC GAlib Boost::unit_test_framework)

add_test(NAME "${PROJECT_NAME}Test" COMMAND "${PROJECT_NAME}Test")
