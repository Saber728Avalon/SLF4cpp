cmake_minimum_required(VERSION 2.8)

project( SLF4cpp 
	LANGUAGES C CXX)

include_directories(../SLF4cpp)
include_directories(../Simple)

#生成调试用exe
OPTION(BUILD_TEST_EXECUTABLES "build exe" ON)

set(SLE4cpp_HDRS
	../SLF4cpp/config.h
	../SLF4cpp/slf_cast.h
	../SLF4cpp/slf_enum.h
	../SLF4cpp/slf_format.h
	../SLF4cpp/slf_log_provider.h
	../SLF4cpp/slf_main.h
	../SLF4cpp/slf_memory.h
	../SLF4cpp/slf_string.h
	../SLF4cpp/slf_time_format.h
	../SLF4cpp/slf_utils.h
	../SLF4cpp/slf4cpp.h
	../SLF4cpp/templae_func.h
)


add_executable(TestSLF4cpp SLF4cpp.cpp 
	stdafx.cpp ${SLE4cpp_HDRS})
	
	