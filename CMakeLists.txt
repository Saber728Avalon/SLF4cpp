project( SLF4cpp 
	LANGUAGES C CXX)
	
cmake_minimum_required(VERSION 2.8)


IF(CMAKE_COMPILER_IS_GNUCXX)
	add_compile_options(-std=c++11)
ENDIF()	

OPTION(BUILD_TEST_EXECUTABLES "build exe" ON)

if(BUILD_TEST_EXECUTABLES)
	add_subdirectory(Test)
endif()