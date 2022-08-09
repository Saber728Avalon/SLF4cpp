// SLF4cpp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "slf4cpp.h"
#include "slf_format.h"
#include "slf_string.h"
#include <assert.h>
#include "templae_func.h"
#include <vector>
#include <algorithm>

void test_cast_8()
{
	int8_t nA = 0, nB = 255, nC = 256, nD = -45;
	SLF4cpp::slf_string strNA = SLF4cpp::slf_cast(nA);
	SLF4cpp::slf_string strNB = SLF4cpp::slf_cast(nB);
	SLF4cpp::slf_string strNC = SLF4cpp::slf_cast(nC);
	SLF4cpp::slf_string strND = SLF4cpp::slf_cast(nD);
	assert((strNA == "0"));
	assert((strNB == "-1"));
	assert((strNC == "0"));
	assert((strND == "-45"));

	uint8_t unA = 0, unB = 56, unC = 256, unD = 255;
	SLF4cpp::slf_string strUnA = SLF4cpp::slf_cast(unA);
	SLF4cpp::slf_string strUnB = SLF4cpp::slf_cast(unB);
	SLF4cpp::slf_string strUnC = SLF4cpp::slf_cast(unC);
	SLF4cpp::slf_string strUnD = SLF4cpp::slf_cast(unD);
	assert((strUnA == "0"));
	assert((strUnB == "56"));
	assert((strUnC == "0"));
	assert((strUnD == "255"));
}

void test_cast_16()
{
	int16_t nA = 0x0000, nB = 0xFFFF, nC = 0xE456, nD = 0x7ad8;
	SLF4cpp::slf_string strNA = SLF4cpp::slf_cast(nA);
	SLF4cpp::slf_string strNB = SLF4cpp::slf_cast(nB);
	SLF4cpp::slf_string strNC = SLF4cpp::slf_cast(nC);
	SLF4cpp::slf_string strND = SLF4cpp::slf_cast(nD);
	assert((strNA == "0"));
	assert((strNB == "-1"));
	assert((strNC == "-7082"));
	assert((strND == "31448"));

	uint16_t unA = 0, unB = 0xFF15, unC = 0xFFFF, unD = 0x4fff;
	SLF4cpp::slf_string strUnA = SLF4cpp::slf_cast(unA);
	SLF4cpp::slf_string strUnB = SLF4cpp::slf_cast(unB);
	SLF4cpp::slf_string strUnC = SLF4cpp::slf_cast(unC);
	SLF4cpp::slf_string strUnD = SLF4cpp::slf_cast(unD);
	assert((strUnA == "0"));
	assert((strUnB == "65301"));
	assert((strUnC == "65535"));
	assert((strUnD == "20479"));
}
void test_cast_32()
{
	int32_t nA = 0x0000, nB = 0xFFFFFFFF, nC = 0xFFFFE456, nD = 0xFFFF7ad8;
	SLF4cpp::slf_string strNA = SLF4cpp::slf_cast(nA);
	SLF4cpp::slf_string strNB = SLF4cpp::slf_cast(nB);
	SLF4cpp::slf_string strNC = SLF4cpp::slf_cast(nC);
	SLF4cpp::slf_string strND = SLF4cpp::slf_cast(nD);
	assert((strNA == "0"));
	assert((strNB == "-1"));
	assert((strNC == "-7082"));
	assert((strND == "-34088"));

	uint32_t unA = 0, unB = 0xFFFFFF15, unC = 0xFFFF0000, unD = 0x4fff000;
	SLF4cpp::slf_string strUnA = SLF4cpp::slf_cast(unA);
	SLF4cpp::slf_string strUnB = SLF4cpp::slf_cast(unB);
	SLF4cpp::slf_string strUnC = SLF4cpp::slf_cast(unC);
	SLF4cpp::slf_string strUnD = SLF4cpp::slf_cast(unD);
	assert((strUnA == "0"));
	assert((strUnB == "4294967061"));
	assert((strUnC == "4294901760"));
	assert((strUnD == "83881984"));
}
void test_cast_64()
{
	int64_t nA = 0x0000FFFFF, nB = 0xFFFFFFFFFFFFFFFF, nC = 0xFFFFFFFFE456, nD = 0xFFFFFFFF7ad8;
	SLF4cpp::slf_string strNA = SLF4cpp::slf_cast(nA);
	SLF4cpp::slf_string strNB = SLF4cpp::slf_cast(nB);
	SLF4cpp::slf_string strNC = SLF4cpp::slf_cast(nC);
	SLF4cpp::slf_string strND = SLF4cpp::slf_cast(nD);
	assert((strNA == "1048575"));
	assert((strNB == "-1"));
	assert((strNC == "281474976703574"));
	assert((strND == "281474976676568"));

	uint64_t unA = 0, unB = 0xFF15FFFFFFFF, unC = 0xFFFFFFFFFFFF, unD = 0x4fffFFFFFFFF;
	SLF4cpp::slf_string strUnA = SLF4cpp::slf_cast(unA);
	SLF4cpp::slf_string strUnB = SLF4cpp::slf_cast(unB);
	SLF4cpp::slf_string strUnC = SLF4cpp::slf_cast(unC);
	SLF4cpp::slf_string strUnD = SLF4cpp::slf_cast(unD);
	assert((strUnA == "0"));
	assert((strUnB == "280469954363391"));
	assert((strUnC == "281474976710655"));
	assert((strUnD == "87960930222079"));
}


void TestFormat()
{
	//test format
	SLF4cpp::slf_format format;
	SLF4cpp::slf_string strRst;

	//解析参数为空
	strRst = SLF4cpp::slf_to_string_args("", (SLF4cpp::AnalyzeParam *)NULL, 0, 0, 123, 456, "xxxx");
	assert(strRst == "");

	//整数十进制
	strRst = format.format("xxxx {8} {07} {1} {} xxxx", 14789, 25565, 37889, -1, "yyyyy  aaaa", 'c'); //参数多于描述，应该不出现
	assert(strRst == "xxxx    14789 0025565 37889 -1 xxxx");

	strRst = format.format("xxxx {06} {3} {} {} {} {} {}xxxx", 14789, -1, "yyyyy  aaaa", 'c'); //描述多于参数，应该截断
	assert(strRst == "xxxx 014789  -1 yyyyy  aaaa 99");

	//整数十六进制
	strRst = format.format("xxxx {:x 04} {:x 8} {:x 020} {} xxxx", 0xFFC5, 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF, -1, "yyyyy  aaaa", 'c'); //参数多于描述，应该不出现
	assert(strRst == "xxxx FFC5 FFFFFFFF 0000FFFFFFFFFFFFFFFF -1 xxxx");

	strRst = format.format("xxxx 0x{:x 8} 0x{:x} 0x{:x} 0x{:x} 0x{:x} {:x} {}xxxx", 14789, -1, "yyyyy  aaaa", 'c'); //描述多于参数，应该截断
	assert(strRst == "xxxx 0x    39C5 0xFFFFFFFFFFFFFFFF 0xyyyyy  aaaa 0x63");

	//float
	strRst = format.format("xxxx {020.2} {8.3} {08.3} {}, {08.5} {} xxxx", 14789.98f, 99.99999, 99.99999, -1, 14789.98f,  "yyyyy  aaaa", 'c'); //参数多于描述，应该不出现
	assert(strRst == "xxxx 00000000000000014789.98     100.000 00000100.000 -1, 00014789.98047 yyyyy  aaaa xxxx");

	//test log
	SLF4CPP(DEBUG)("xxxx{x 0 4 4} {x 0 4 4} {x 0 4} {}xxxx", 14789, 25565, 37889, -1, "yyyyy  aaaa", 'c'); //参数多于描述，应该不出现
	SLF4CPP(DEBUG)("xxxx{x 0 4 4} {} {} {} {} {} {}xxxx", 14789, -1, "yyyyy  aaaa", 'c'); //描述多于参数，应该不出现
}

void TestLog()
{
}

int main()
{
	SLF4CPP_INIT(0);
	SLF4cpp::slf_log_provider_stdout xxx;

	float f = 200.5f;
	float *pxxx = &f;


	TestFormat();
	TestLog();

	test_cast_8();
	test_cast_16();
	test_cast_32();
	test_cast_64();
    return 0;
}

