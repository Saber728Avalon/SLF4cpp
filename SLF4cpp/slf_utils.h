#pragma once
#include <iostream>
#include <time.h>
#if defined(_WIN32) || defined(_WIN64)
#include <stdint.h>
#include <Windows.h>
#endif


struct SLF_MT
{ 
	int tm_millsec;
	int tm_sec;  
	int tm_min;     
	int tm_hour;    
	int tm_mday;    
	int tm_mon;    
	int tm_year;    
};

inline void slf_mem_copy(void *pDst, const void *pSrc, const int nLen)
{
	memcpy(pDst, pSrc, nLen);
}


inline void slf_mem_set(char *pDst, const char value, const int nLen)
{
	memset(pDst, value, nLen);
}


inline struct SLF_MT slf_time_stamp()
{
	time_t rawtime;
	struct tm timeInfo;
	struct SLF_MT slfMT;

	time(&rawtime);
	localtime_s(&timeInfo, &rawtime);

	slfMT.tm_millsec = rawtime % 1000;
	slfMT.tm_year = timeInfo.tm_year + 1900;
	slfMT.tm_mon = timeInfo.tm_mon + 1;
	slfMT.tm_mday = timeInfo.tm_mday;
	slfMT.tm_hour = timeInfo.tm_hour + 1;
	slfMT.tm_min = timeInfo.tm_min;
	slfMT.tm_sec = timeInfo.tm_sec;
	return slfMT;
}

uint32_t GetCurrentPID()
{
	uint32_t un32PID = 0;
#if defined(_WIN32) || defined(_WIN64)
	un32PID = GetCurrentThreadId();
#else
#endif
	return un32PID;
}