#pragma once
#include <iostream>
#include <time.h>
#if defined(_WIN32) || defined(_WIN64)
#include <stdint.h>
#endif

inline void slf_mem_copy(char *pDst, const char *pSrc, const int nLen)
{
	memcpy(pDst, pSrc, nLen);
}


inline void slf_mem_set(char *pDst, const char value, const int nLen)
{
	memset(pDst, value, nLen);
}


inline struct tm slf_time_stamp()
{
	time_t rawtime;
	struct tm timeInfo;

	time(&rawtime);
	localtime_s(&timeInfo, &rawtime);

	return timeInfo;
}
