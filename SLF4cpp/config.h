//#pragma once

// some function use std lib;
#if !defined(USE_STD_FUNC)
#define USE_STD_FUNC 
#endif

//日志末尾是否添加\n
#if !defined(LOG_END_RETURN)
#define LOG_END_RETURN 
#endif

//删除日志，启动该宏之后。会删除所有日志。
#if !defined(REMOVE_LOG_INFO)
#else
#define REMOVE_LOG_INFO
#endif

//设置字符表示单位
#if !defined(SLF4CPP_USE_CHAR) && !defined(SLF4CPP_USE_UTF16) && !defined(SLF4CPP_USE_UTF32) 
#define SLF4CPP_USE_CHAR
#endif
