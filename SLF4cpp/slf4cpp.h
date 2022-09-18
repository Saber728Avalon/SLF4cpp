#pragma once

#include "config.h"
#include "slf_enum.h"
#include "slf_log_provider.h"
#include "slf_main.h"
#include "slf_format.h"
#include "slf_string.h"


#if !defined(REMOVE_LOG_INFO)

//初始化SLF4cpp
#define SLF4CPP_INIT(type)\
SLF4cpp::slf_main::slf4cpp_init(type)

//初始化SLF4cpp日志前缀
#define SLF4CPP_PREFFIX(preffix)\
SLF4cpp::slf_main::slfcpp_prefix(preffix)

//设置新的日志框架
#define SLF4CPP_SET_LOG_PROVIDER(provider)\
SLF4cpp::slf_main::replace_log_provider(provider)


#if !defined(REMOVE_DEBUG_LOG_INFO)
#define SLF4CPP_IMPL_DEBUG(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_main::write_log(SLF4cpp::slf_log_level::slf_log_level_debug, log);\
} while(0)
#else
#define SLF4CPP_IMPL_DEBUG(...) {}
#endif


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


//普通日志输出
#define SLF4CPP(level) SLF4CPP_IMPL_##level


#if !defined(REMOVE_DEBUG_LOG_INFO)
#define SLF4CPP_LINE_IMPL_DEBUG(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_string line = format.format("[{}({})] ", __FUNCTION__, __LINE__);\
SLF4cpp::slf_main::write_log_func_line(SLF4cpp::slf_log_level::slf_log_level_debug, log, line);\
} while(0)
#else
#define SLF4CPP_LINE_IMPL_DEBUG(...) {}
#endif

#define SLF4CPP_LINE_IMPL_INFO(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_string line = format.format("[{}({})] ", __FUNCTION__, __LINE__);\
SLF4cpp::slf_main::write_log_func_line(SLF4cpp::slf_log_level::slf_log_level_info, log, line);\
} while(0)

#define SLF4CPP_LINE_IMPL_WARN(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_string line = format.format("[{}({})] ", __FUNCTION__, __LINE__);\
SLF4cpp::slf_main::write_log_func_line(SLF4cpp::slf_log_level::slf_log_level_warn, log, line);\
} while(0)

#define SLF4CPP_LINE_IMPL_ERROR(...)\
do {\
SLF4cpp::slf_format format;\
SLF4cpp::slf_string log = format.format(__VA_ARGS__);\
SLF4cpp::slf_string line = format.format("[{}({})] ", __FUNCTION__, __LINE__);\
SLF4cpp::slf_main::write_log_func_line(SLF4cpp::slf_log_level::slf_log_level_error, log, line);\
} while(0)

//增加函数名和行数
#define SLF4CPPL(level) SLF4CPP_LINE_IMPL_##level

#else

#define SLF4CPP_INIT(type) {}

#define SLF4CPP_PREFFIX(preffix){}

#define SLF4CPP_SET_LOG_PROVIDER(provider){}

//正常日志输出
#define SLF4CPP_IMPL_DEBUG(...){}
#define SLF4CPP_IMPL_INFO(...){}
#define SLF4CPP_IMPL_WARN(...){}
#define SLF4CPP_IMPL_ERROR(...){}
#define SLF4CPP(level) SLF4CPP_IMPL_##level

//函数名和行数的屏蔽
#define SLF4CPP_LINE_IMPL_DEBUG(...) {}
#define SLF4CPP_LINE_IMPL_INFO(...) {}
#define SLF4CPP_LINE_IMPL_WARN(...) {}
#define SLF4CPP_LINE_IMPL_ERROR(...) {}
#define SLF4CPPL(level) SLF4CPP_LINE_IMPL_##level

#endif //end REMOVE_LOG_INFO