#pragma once

#include "config.h"
#include "slf_enum.h"
#include "slf_log_provider.h"
#include "slf_main.h"
#include "slf_format.h"
#include "slf_string.h"


//初始化SLF4cpp
#define SLF4CPP_INIT(type)\
SLF4cpp::slf_main::slf4cpp_init(type)

//初始化SLF4cpp日志前缀
#define SLF4CPP_PREFFIX(preffix)\
SLF4cpp::slf_main::slfcpp_prefix(preffix)

//设置新的日志框架
#define SLF4CPP_SET_LOG_PROVIDER(provider)\
SLF4cpp::slf_main::replace_log_provider(provider)

#define SLF4CPP_IMPL_DEBUG(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_main::write_log(SLF4cpp::slf_log_level::slf_log_level_debug, log);\
} while(0)


#define SLF4CPP_IMPL_INFO(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_main::write_log(SLF4cpp::slf_log_level::slf_log_level_info, log);\
} while(0)


#define SLF4CPP_IMPL_WARN(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_main::write_log(SLF4cpp::slf_log_level::slf_log_level_warn, log);\
} while(0)


#define SLF4CPP_IMPL_ERROR(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_main::write_log(SLF4cpp::slf_log_level::slf_log_level_error, log);\
} while(0)


#define SLF4CPP(level) SLF4CPP_IMPL_##level

