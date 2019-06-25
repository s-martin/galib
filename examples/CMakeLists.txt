include_directories(${CMAKE_SOURCE_DIR}/ga)
include_directories(${Boost_INCLUDE_DIRS})
set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)

add_executable(example1 ex1.C)
target_compile_features(example1 PUBLIC cxx_std_17)
target_link_libraries(example1 PUBLIC GAlib Boost::program_options)

add_executable(example2 ex2.C)
target_compile_features(example2 PUBLIC cxx_std_17)
target_link_libraries(example2 PUBLIC GAlib Boost::program_options)

add_executable(example3 ex3.C)
target_compile_features(example3 PUBLIC cxx_std_17)
target_link_libraries(example3 PUBLIC GAlib Boost::program_options)

add_executable(example4 ex4.C)
target_compile_features(example4 PUBLIC cxx_std_17)
target_link_libraries(example4 PUBLIC GAlib Boost::program_options)

add_executable(example5 ex5.C)
target_compile_features(example5 PUBLIC cxx_std_17)
target_link_libraries(example5 PUBLIC GAlib Boost::program_options)

# add_executable(example6 ex6.C)
# target_compile_features(example6 PUBLIC cxx_std_17)
# target_link_libraries(example6 PUBLIC GAlib)

add_executable(example7 ex7.C)
target_compile_features(example7 PUBLIC cxx_std_17)
target_link_libraries(example7 PUBLIC GAlib Boost::program_options)

# add_executable(example8 ex8.C)
# target_compile_features(example8 PUBLIC cxx_std_17)
# target_link_libraries(example8 PUBLIC GAlib)

add_executable(example9 ex9.C)
target_compile_features(example9 PUBLIC cxx_std_17)
target_link_libraries(example9 PUBLIC GAlib Boost::program_options)

if(NOT MSVC) 
	add_executable(gnuex 
					./gnu/gnuex.C 
					./gnu/bitstr.C
					./gnu/AllocRing.cc 
					./gnu/Obstack.cc 
					./gnu/BitString.cc 
					./gnu/builtin.cc 
					./gnu/bitand.c 
					./gnu/bitany.c 
					./gnu/bitblt.c 
					./gnu/bitclear.c 
					./gnu/bitcopy.c 
					./gnu/bitcount.c 
					./gnu/bitinvert.c 
					./gnu/bitlcomp.c 
					./gnu/bitset1.c 
					./gnu/bitxor.c 
					./gnu/error.cc )
	target_compile_features(gnuex PUBLIC cxx_std_17)
	target_link_libraries(gnuex GAlib)
	target_include_directories(gnuex PUBLIC ${CMAKE_SOURCE_DIR}/examples/gnu)
endif()

if(X11_FOUND) 
	add_executable(gaview 
					./graphic/gaview.C )
	target_compile_features(gaview PUBLIC cxx_std_17)
	target_link_libraries(gaview GAlib Xaw ${X11_Xt_LIB} ${X11_LIBRARIES})
	target_include_directories(gaview PUBLIC ${X11_INCLUDE_DIR})
	
	#add_executable(tspview  
	#				./graphic/tspview.C)
	#target_compile_features(tspview PUBLIC cxx_std_17)
	#target_link_libraries(tspview GAlib Xaw ${X11_Xt_LIB} ${X11_LIBRARIES})
	#target_include_directories(tspview PUBLIC ${X11_INCLUDE_DIR})
endif()

# add_executable(pvmpop 
#                 ./pvmpop/genome.C 
#                 ./pvmpop/genome.h 
#                 ./pvmpop/master.C
#                 ./pvmpop/PVMDemeGA.C 
#                 ./pvmpop/PVMDemeGA.h
#                 ./pvmpop/slave.C)
# target_compile_features(pvmpop PUBLIC cxx_std_17)
# target_link_libraries(pvmpop PUBLIC GAlib)
