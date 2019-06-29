include_directories(${CMAKE_SOURCE_DIR}/ga)
include_directories(${Boost_INCLUDE_DIRS})
set_property(DIRECTORY PROPERTY CXX_EXTENSIONS OFF)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example1 ex1.C)
target_link_libraries(example1 PUBLIC GAlib)

add_executable(example2 ex2.C)
target_link_libraries(example2 PUBLIC GAlib)

add_executable(example3 ex3.C)
target_link_libraries(example3 PUBLIC GAlib)

add_executable(example4 ex4.C)
target_link_libraries(example4 PUBLIC GAlib)

add_executable(example5 ex5.C)
target_link_libraries(example5 PUBLIC GAlib)

add_executable(example6 ex6.C)
target_link_libraries(example6 PUBLIC GAlib)

add_executable(example7 ex7.C)
target_link_libraries(example7 PUBLIC GAlib)

add_executable(example8 ex8.C)
target_link_libraries(example8 PUBLIC GAlib)

add_executable(example9 ex9.C)
target_link_libraries(example9 PUBLIC GAlib)

add_executable(example10 ex10.C)
target_link_libraries(example10 PUBLIC GAlib)

add_executable(example11 ex11.C)
target_link_libraries(example11 PUBLIC GAlib)

#add_executable(example12 ex12.C)
#target_link_libraries(example12 PUBLIC GAlib)

add_executable(example13 ex13.C)
target_link_libraries(example13 PUBLIC GAlib)

add_executable(example14 ex14.C)
target_link_libraries(example14 PUBLIC GAlib)

add_executable(example15 ex15.C)
target_link_libraries(example15 PUBLIC GAlib)

add_executable(example16 ex16.C)
target_link_libraries(example16 PUBLIC GAlib)

#add_executable(example17 ex17.C)
#target_link_libraries(example17 PUBLIC GAlib)

add_executable(example18 ex18.C)
target_link_libraries(example18 PUBLIC GAlib)


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
	target_link_libraries(gnuex GAlib)
	target_include_directories(gnuex PUBLIC ${CMAKE_SOURCE_DIR}/examples/gnu)
endif()

if(X11_FOUND) 
	add_executable(gaview 
					./graphic/gaview.C )
	target_link_libraries(gaview GAlib Xaw ${X11_Xt_LIB} ${X11_LIBRARIES})
	target_include_directories(gaview PUBLIC ${X11_INCLUDE_DIR})
	
	#add_executable(tspview  
	#				./graphic/tspview.C)
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
# target_link_libraries(pvmpop PUBLIC GAlib)
