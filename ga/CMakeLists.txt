

file(GLOB SRCS *.C) # TODO remove GLOB
file(GLOB HDRS *.h) # TODO remove GLOB
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/ga)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem)