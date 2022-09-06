cmake_minimum_required(VERSION 2.8)

project( SLF4cpp 
	LANGUAGES C CXX)

OPTION(BUILD_TEST_EXECUTABLES "build exe" ON)

if(BUILD_TEST_EXECUTABLES)
	add_subdirectory(Test)
endif()