


add_executable("${PROJECT_NAME}Test" 
        "main.cpp"
        "GASimpleGATest.cpp")

target_include_directories("${PROJECT_NAME}Test" PUBLIC "../ga")

target_link_libraries("${PROJECT_NAME}Test" PUBLIC GAlib-NG Boost::unit_test_framework)

add_test(NAME "${PROJECT_NAME}Test" COMMAND "${PROJECT_NAME}Test")




