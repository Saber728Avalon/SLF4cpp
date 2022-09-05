#pragma once
#include <stdarg.h>
#include "slf_string.h"
#include "slf_cast.h"


namespace SLF4cpp
{
#define DEF_FORMAT_BUF_SIZE 10
#define DEF_FORMAT_MAX_ITEM 40
#define DEF_FLOAT_DECIMAL_BUF_SIZE 128

#define SLF_STRING_ARGS slf_string strRst;\
		if (NULL == analyzeParam || nPos >= nMaxParamCount){\
			return strRst;\
		}\
		slf_string strTmp;\
		if (0 == analyzeParam[nPos].charOutputType){\
			strTmp = slf_to_string(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);\
		}\
		else if ('x' == analyzeParam[nPos].charOutputType){\
			strTmp = slf_to_string_hex(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);\
		}\
		if (0 == nPos){\
			strRst.append(_Format, analyzeParam[nPos].unStart);\
			strRst.append(strTmp);\
		}\
		else if (nMaxParamCount - 1 == nPos){\
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1\
				, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);\
			strRst.append(strTmp);\
			strRst.append(_Format + analyzeParam[nPos].unEnd + 1);\
		}\
		else {\
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);\
			strRst.append(strTmp);\
		}\

	struct AnalyzeParam
	{
		AnalyzeParam()
		{
			charAlign = ' ';
			unAlignInteger = 0;
			unAlignDecimal = 0;
			charOutputType = 0;
			unStart = 0;
			unEnd = 0;
		}
		slf_string::PROTOTYPE charOutputType;
		slf_string::PROTOTYPE charAlign;
		uint32_t unAlignInteger;
		uint32_t unAlignDecimal;

		uint32_t unStart;
		uint32_t unEnd;
	};

	slf_string float_format_length(const slf_string &strNo
		, const slf_string::PROTOTYPE charAlign
		, const int nIntegerAlign
		, const int nDecimalAlign)
	{
		int nPosPoint = 0;
		int nSub = 0;
		slf_string::PROTOTYPE chRstBuf[DEF_FLOAT_DECIMAL_BUF_SIZE] = { 0 };
		slf_string::PROTOTYPE *pChRstBufCur = chRstBuf + 1;// to round up or down, maybe add 1 word
		slf_string strFormatRst(strNo);

		if (nDecimalAlign >= DEF_FLOAT_DECIMAL_BUF_SIZE)
		{
			return strFormatRst;
		}

		//find point
		for (int i = 0; i < strNo.length(); i++)
		{
			if ('.' == strNo[i])
			{
				nPosPoint = i;
				break;
			}
		}

		if (0 == nPosPoint)
		{
			return strFormatRst;
		}

		//Align Integer
		nSub = nIntegerAlign - nPosPoint;
		if (nSub > 0)
		{
			for (int i = 0; i < nSub; i++)
			{
				pChRstBufCur[0] = charAlign;
				pChRstBufCur++;
			}
		}
		for (int i = 0; i <= nPosPoint; i++)
		{
			pChRstBufCur[0] = strNo[i];
			pChRstBufCur++;
		}

		//Align Decimal
		nSub = nDecimalAlign - (strNo.length() - nPosPoint - 1);
		if (nSub >= 0)//need append
		{
			for (int i = nPosPoint + 1; i < strNo.length(); i++)
			{
				pChRstBufCur[0] = strNo[i];
				pChRstBufCur++;
			}
			for (; nSub > 0; nSub--)
			{
				pChRstBufCur[0] = charAlign;
				pChRstBufCur++;
			}
		}
		else
		{
			int nPosDecimal = nPosPoint + 1;
			const int nPosEnd = nPosPoint + 1 + nDecimalAlign;

			for (; nPosDecimal < nPosEnd; nPosDecimal++)
			{
				pChRstBufCur[0] = strNo[nPosDecimal];
				pChRstBufCur++;
			}
			if (strNo[nPosDecimal] >= '5' && strNo[nPosDecimal] <= '9')
			{
				for (pChRstBufCur--; pChRstBufCur >= chRstBuf; pChRstBufCur--)
				{
					if ((pChRstBufCur[0] <= '9' && pChRstBufCur[0] >= '0')
						)
					{
						if ('9' == pChRstBufCur[0])
						{
							pChRstBufCur[0] = '0';
						}
						else
						{
							pChRstBufCur[0] = pChRstBufCur[0] + 1;
							break;
						}
					}
					else if (pChRstBufCur[0] == charAlign
						|| '\0' == pChRstBufCur[0])
					{
						pChRstBufCur[0] = '1';
						break;
					}
				}
			}
		}

		if ('\0' == chRstBuf[0])
		{
			strFormatRst = chRstBuf + 1;
		}
		else
		{
			strFormatRst = chRstBuf + 0;
		}

		return std::move(strFormatRst);
	}

	template <typename T>
	slf_string slf_to_string(const T& value
		, const slf_string::PROTOTYPE charAlign
		, const int nIntegerAlign
		, const int nDecimalAlign)
	{
		slf_string strRst = slf_cast(value);

		// float number
		if (nDecimalAlign > 0)
		{
			strRst = float_format_length(strRst, charAlign, nIntegerAlign, nDecimalAlign);
		}
		else
		{
			int nSub = nIntegerAlign - strRst.length();
			if (nSub > 0)
			{
				slf_string strTmpAlign;
				for (; nSub > 0; nSub--)
				{
					strTmpAlign.append(charAlign);
				}
				strRst = strTmpAlign + strRst;
			}
		}


		return std::move(strRst);
	}

	slf_string slf_to_string(const slf_string::PROTOTYPE * value
		, const slf_string::PROTOTYPE charAlign
		, const int nIntegerAlign
		, const int nDecimalAlign)
	{
		slf_string strRst;
		strRst.append(value);
		return std::move(strRst);
	}

	template <typename T>
	slf_string slf_to_string_hex(const T& value
		, const slf_string::PROTOTYPE charAlign
		, const int nIntegerAlign
		, const int nDecimalAlign)
	{
		slf_string strRst = slf_cast_hex(value);
		int nSub = nIntegerAlign - strRst.length();
		if (nSub > 0)
		{
			slf_string strTmpAlign;
			for (; nSub > 0; nSub--)
			{
				strTmpAlign.append(charAlign);
			}
			strRst = strTmpAlign + strRst;
		}
		return std::move(strRst);
	}

	slf_string slf_to_string_hex(const slf_string::PROTOTYPE * value
		, const slf_string::PROTOTYPE charAlign
		, const int nIntegerAlign
		, const int nDecimalAlign)
	{
		slf_string strRst;
		strRst.append(value);
		return std::move(strRst);
	}


	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos)
	{
		slf_string strRst;
		return std::move(strRst);
	}


#if defined(_WIN32) || defined(_WIN64)
#if _MSC_VER > 1600
	template <typename T, typename...Args>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos, const T& value, const Args&... args)
	{
		slf_string strRst;

		if (NULL == analyzeParam || nPos >= nMaxParamCount)
		{
			return strRst;
		}
		slf_string strTmp;
		if (0 == analyzeParam[nPos].charOutputType)
		{
			strTmp = slf_to_string(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);
		}
		else if ('x' == analyzeParam[nPos].charOutputType)
		{
			strTmp = slf_to_string_hex(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);
		}

		if (0 == nPos)
		{
			strRst.append(_Format, analyzeParam[nPos].unStart);
			strRst.append(strTmp);
		}
		else if (nMaxParamCount - 1 == nPos)
		{
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1
				, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);
			strRst.append(strTmp);
			strRst.append(_Format + analyzeParam[nPos].unEnd + 1);
		}
		else
		{
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);
			strRst.append(strTmp);
		}

		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, args...);
		strRst.append(strNext);
		return  std::move(strRst);
	}
#else
	template <typename T>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos, const T& value)
	{
		slf_string strRst;

		if (NULL == analyzeParam || nPos >= nMaxParamCount)
		{
			return strRst;
		}
		slf_string strTmp;
		if (0 == analyzeParam[nPos].charOutputType)
		{
			strTmp = slf_to_string(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);
		}
		else if ('x' == analyzeParam[nPos].charOutputType)
		{
			strTmp = slf_to_string_hex(value, analyzeParam[nPos].charAlign, analyzeParam[nPos].unAlignInteger, analyzeParam[nPos].unAlignDecimal);
		}

		if (0 == nPos)
		{
			strRst.append(_Format, analyzeParam[nPos].unStart);
			strRst.append(strTmp);
		}
		else if (nMaxParamCount - 1 == nPos)
		{
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1
				, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);
			strRst.append(strTmp);
			strRst.append(_Format + analyzeParam[nPos].unEnd + 1);
		}
		else
		{
			strRst.append(_Format + analyzeParam[nPos - 1].unEnd + 1, analyzeParam[nPos].unStart - analyzeParam[nPos - 1].unEnd - 1);
			strRst.append(strTmp);
		}

		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos, const T& value, const T1& value1)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11, typename T12>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11, const T12& value12)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11, value12);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11, typename T12, typename T13>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11, const T12& value12, const T13& value13)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11, value12, value13);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11, typename T12, typename T13, typename T14>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11, const T12& value12, const T13& value13, const T14& value14)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11, value12, value13, value14);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11, const T12& value12, const T13& value13, const T14& value14
		, const T15& value15)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11, value12, value13, value14, value15);
		strRst.append(strNext);
		return  std::move(strRst);
	}
	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9
	, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
	slf_string slf_to_string_args(const slf_string::PROTOTYPE *_Format, const AnalyzeParam analyzeParam[], int nMaxParamCount, int nPos
		, const T& value, const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7
		, const T8& value8, const T9& value9, const T10& value10, const T11& value11, const T12& value12, const T13& value13, const T14& value14
		, const T15& value15, const T16& value16)
	{
		SLF_STRING_ARGS;
		slf_string strNext = slf_to_string_args(_Format, analyzeParam, nMaxParamCount, nPos += 1, value1, value2, value3, value4, value5, value6
			, value7, value8, value9, value10, value11, value12, value13, value14, value15, value16);
		strRst.append(strNext);
		return  std::move(strRst);
	}
#endif

#endif

	class slf_format
	{
		enum FORMAT_STATE
		{
			FORMAT_STATE_INIT,
			FORMAT_STATE_APPEND,
			FORMAT_STATE_APPEND_FORMAT,

			FORMAT_STATE_COUNT
		};


	public:

		static void InitFormat()
		{
		}


		//format("afdalskdjfa alkdsfja;lsd{0.4d}")
#if defined(_WIN32) || defined(_WIN64)
#if _MSC_VER > 1600
		template <typename...Args>
		slf_string format(const slf_string::PROTOTYPE *_Format, const Args&... args)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, args...);
			return std::move(strRst);
		}
#else
		slf_string format(const slf_string::PROTOTYPE *_Format)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0);
			return std::move(strRst);
		}
		template <typename T1>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1);
			return std::move(strRst);
		}
		template <typename T1, typename T2>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3, const T4 &value4)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11, typename T12>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11, const T12 &value12)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11, value12);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11, typename T12, typename T13>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11, const T12 &value12, const T13 &value13)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11, value12, value13);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11, typename T12, typename T13, typename T14>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11, const T12 &value12, const T13 &value13, const T14 &value14)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11, value12, value13, value14);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11, typename T12, typename T13, typename T14, typename T15>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11, const T12 &value12, const T13 &value13, const T14 &value14, const T15 &value15)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11, value12, value13, value15);
			return std::move(strRst);
		}
		template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
		, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
		slf_string format(const slf_string::PROTOTYPE *_Format, const T1 &value1, const T2 &value2, const T3 &value3
			, const T4 &value4, const T5 &value5, const T6 &value6, const T7 &value7, const T8 &value8, const T9 &value9, const T10 &value10
			, const T11 &value11, const T12 &value12, const T13 &value13, const T14 &value14, const T15 &value15, const T16 &value16)
		{
			slf_string strRst = _Format;
			AnalyzeParam analyzeParam[DEF_FORMAT_MAX_ITEM];
			int nParamCount = format_internal(_Format, analyzeParam, DEF_FORMAT_MAX_ITEM);
			strRst = slf_to_string_args(_Format, analyzeParam, nParamCount, 0, value1, value2, value3, value4, value5, value6, value7, value8, value10
				, value11, value12, value13, value15, value16);
			return std::move(strRst);
		}
#endif// _MSC_VER > 1600
#endif //defined(_WIN32) || defined(_WIN64)

		int format_internal(const slf_string::PROTOTYPE *_Format, AnalyzeParam analyzeParam[], int nMaxParam)
		{
			int nAnalyzeParamPos = 0;

			slf_string::PROTOTYPE charTypeBuf[DEF_FORMAT_BUF_SIZE];
			slf_string::PROTOTYPE charTmpBuf[DEF_FORMAT_BUF_SIZE];
			int nTmpBubfPos = 0;

			FORMAT_STATE state = FORMAT_STATE::FORMAT_STATE_APPEND;

			for (int i = 0; _Format[i] != '\0'; )
			{
				if ('{' == _Format[i])
				{
					if ('{' == _Format[i + 1]) // is {
					{
						i++;
					}
					else
					{
						state = FORMAT_STATE::FORMAT_STATE_APPEND_FORMAT;
						analyzeParam[nAnalyzeParamPos].unStart = i;
					}
				}
				else if ('}' == _Format[i])
				{
					if ('}' == _Format[i + 1]) // is }
					{
						//m_strMsg.append(slf_string::PROTOTYPE('}'));
						i++;
					}
					else
					{
						// set initialize
						state = FORMAT_STATE::FORMAT_STATE_APPEND;
						analyzeParam[nAnalyzeParamPos].unEnd = i;
						nAnalyzeParamPos++;
						if (nAnalyzeParamPos >= nMaxParam)
						{
							//assert;
							return nAnalyzeParamPos;
						}
					}
				}
				else if (FORMAT_STATE::FORMAT_STATE_APPEND == state)
				{
				}
				else
				{
					if (!IsShowChar(_Format[i]))
					{
						state = FORMAT_STATE(state + 1);
					}
					else
					{
						if (':' == _Format[i])
						{
							analyzeParam[nAnalyzeParamPos].charOutputType = _Format[i + 1];
							i++;
						}
						else
						{
							nTmpBubfPos = 0;
							if ('0' == _Format[i])
							{
								analyzeParam[nAnalyzeParamPos].charAlign = '0';
								i++;
							}

							while (_Format[i] != '\0' &&  IsShowChar(_Format[i]))
							{
								if ('.' == _Format[i])
								{
									charTmpBuf[nTmpBubfPos++] = '\0';
									analyzeParam[nAnalyzeParamPos].unAlignInteger = (uint16_t)slf_cast_un(charTmpBuf);
									nTmpBubfPos = 0;
									i++;
								}
								charTmpBuf[nTmpBubfPos++] = _Format[i++];
							}
							charTmpBuf[nTmpBubfPos++] = '\0';
							i--;
							if (0 == analyzeParam[nAnalyzeParamPos].unAlignInteger)
							{
								analyzeParam[nAnalyzeParamPos].unAlignInteger = (uint32_t)slf_cast_un(charTmpBuf);
							}
							else
							{
								analyzeParam[nAnalyzeParamPos].unAlignDecimal = (uint32_t)slf_cast_un(charTmpBuf);
							}
						}
					}
				}
				i++;
			}
			return nAnalyzeParamPos;
		}

	private:
		inline bool IsShowChar(const slf_string::PROTOTYPE &ch)
		{
			if (ch > 0x20)
			{
				if (('{' != ch) && ('}' != ch))
				{
					return true;
				}
			}

			return false;
		}
	};
}