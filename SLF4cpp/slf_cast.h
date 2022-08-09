#pragma once
#include <iostream>
#include "slf_string.h"
#include "config.h"

namespace SLF4cpp
{
	slf_string unsigned_integer_hex(uint64_t un64Value)
	{
		slf_string strRst;
		slf_string::PROTOTYPE arrayEvery[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0' };
		uint64_t un64SrcValue = un64Value;

		for (; un64SrcValue > 0; un64SrcValue >>= 4)
		{
			strRst.append(arrayEvery[un64SrcValue & 0xF]);
		}

		//exchange order
		int nSize = strRst.length() / 2;
		int nTmpSize = strRst.length();
		nTmpSize -= 1;
		for (int i = 0; i < nSize; i++)
		{
			slf_string::PROTOTYPE tmp = strRst[i];
			int nPos = nTmpSize - i;
			strRst[i] = strRst[nPos];
			strRst[nPos] = tmp;
		}

		if (0 == strRst.length())
		{
			strRst.append(slf_string::PROTOTYPE('0'));
		}
		return std::move(strRst);
	}

	slf_string unsigned_integer(uint64_t un64Value)
	{
		uint64_t un64SrcValue = un64Value;
		slf_string strRst;

		for (; un64SrcValue > 0; un64SrcValue /= 10)
		{
			uint8_t un8Number = un64SrcValue % 10;
			un8Number += 0x30;
			strRst.append(un8Number);
		}

		//exchange order
		int nSize = strRst.length() / 2;
		int nTmpSize = strRst.length();
		nTmpSize -= 1;
		for (int i = 0; i < nSize; i++)
		{
			slf_string::PROTOTYPE tmp = strRst[i];
			int nPos = nTmpSize - i;
			strRst[i] = strRst[nPos];
			strRst[nPos] = tmp;
		}

		if (0 == strRst.length())
		{
			strRst.append(slf_string::PROTOTYPE('0'));
		}
		return std::move(strRst);
	}


	int unsigned_integer(uint64_t un64Value, slf_string::PROTOTYPE *pStrBuf, int nLen)
	{
		uint64_t un64SrcValue = un64Value;
		int nPos = 0;
		for (; un64SrcValue > 0; un64SrcValue /= 10)
		{
			uint8_t un8Number = un64SrcValue % 10;
			un8Number += 0x30;

			pStrBuf[nPos] = un8Number;
			nPos++;
		}

		//exchange order
		int nSize = nPos / 2;
		int nTmpSize = nPos;
		nTmpSize -= 1;
		for (int i = 0; i < nSize; i++)
		{
			slf_string::PROTOTYPE tmp = pStrBuf[i];
			int nPos = nTmpSize - i;
			pStrBuf[i] = pStrBuf[nPos];
			pStrBuf[nPos] = tmp;
		}

		return nPos;
	}

	slf_string signed_integer(int64_t n64Value)
	{
		slf_string strRst;
		if (0x8000000000000000 & n64Value)
		{
			n64Value = 0 - n64Value;
			strRst.append(slf_string::PROTOTYPE('-'));
			strRst.append(unsigned_integer(n64Value));
		}
		else
		{
			strRst = unsigned_integer(n64Value);
		}
		return std::move(strRst);
	}

	uint64_t slf_cast_un(const slf_string &str)
	{
		uint64_t un64Rst = 0;
		for (int i = 0; i < str.length(); i++)
		{
			uint32_t unTmpValue = str[i] - slf_string::PROTOTYPE('0');
			un64Rst = un64Rst * 10 + unTmpValue;
		}
		return un64Rst;
	}
	uint64_t slf_cast_un(const slf_string::PROTOTYPE *pStr)
	{
		uint64_t un64Rst = 0;
		for (int i = 0; slf_string::PROTOTYPE('\0') != pStr[i]; i++)
		{
			uint32_t unTmpValue = pStr[i] - slf_string::PROTOTYPE('0');
			un64Rst = un64Rst * 10 + unTmpValue;
		}
		return un64Rst;
	}


	void writeDecimals(uint32_t value, slf_string &strRst) {
		int width = 9;

		// remove trailing zeros
		while (value % 10 == 0 && width > 0) {
			value /= 10;
			width--;
		}

		char buffer[16];
		char *ptr = buffer + sizeof(buffer) - 1;

		// write the string in reverse order
		*ptr = 0;
		while (width--) {
			*--ptr = value % 10 + '0';
			value /= 10;
		}
		*--ptr = '.';

		// and dump it in the right order
		strRst.append(ptr);
	}


	void writeInteger(uint32_t value, slf_string &strRst) {
		char buffer[16] = {0};
		char *ptr = buffer + sizeof(buffer) - 1;

		*ptr = 0;
		do {
			*--ptr = (char)(value % 10) + '0';
			value = (uint32_t)(value / 10);
		} while (value);

		strRst.append(ptr);
	}


	int normalizeFloat(double& value) {
		const double positiveExpThreshold = 1e7;
		const double negativeExpThreshold = 1e-5;
		int exponent = 0;

		if (value >= positiveExpThreshold) {
			if (value >= 1e256) {
				value /= 1e256;
				exponent += 256;
			}
			if (value >= 1e128) {
				value /= 1e128;
				exponent += 128;
			}
			if (value >= 1e64) {
				value /= 1e64;
				exponent += 64;
			}
			if (value >= 1e32) {
				value /= 1e32;
				exponent += 32;
			}
			if (value >= 1e16) {
				value /= 1e16;
				exponent += 16;
			}
			if (value >= 1e8) {
				value /= 1e8;
				exponent += 8;
			}
			if (value >= 1e4) {
				value /= 1e4;
				exponent += 4;
			}
			if (value >= 1e2) {
				value /= 1e2;
				exponent += 2;
			}
			if (value >= 1e1) {
				value /= 1e1;
				exponent += 1;
			}
		}

		if (value > 0 && value <= negativeExpThreshold) {
			if (value < 1e-255) {
				value *= 1e256;
				exponent -= 256;
			}
			if (value < 1e-127) {
				value *= 1e128;
				exponent -= 128;
			}
			if (value < 1e-63) {
				value *= 1e64;
				exponent -= 64;
			}
			if (value < 1e-31) {
				value *= 1e32;
				exponent -= 32;
			}
			if (value < 1e-15) {
				value *= 1e16;
				exponent -= 16;
			}
			if (value < 1e-7) {
				value *= 1e8;
				exponent -= 8;
			}
			if (value < 1e-3) {
				value *= 1e4;
				exponent -= 4;
			}
			if (value < 1e-1) {
				value *= 1e2;
				exponent -= 2;
			}
			if (value < 1e0) {
				value *= 1e1;
				exponent -= 1;
			}
		}

		return exponent;
	}





	void splitFloat(double value, uint32_t &integralPart,
		uint32_t &decimalPart, int16_t &exponent) {
		exponent = normalizeFloat(value);

		integralPart = (uint32_t)value;
		double remainder = value - integralPart;

		remainder *= 1e9;
		decimalPart = (uint32_t)remainder;

		// rounding
		remainder -= decimalPart;
		if (remainder >= 0.5) {
			decimalPart++;
			if (decimalPart >= 1000000000) {
				decimalPart = 0;
				integralPart++;
				if (exponent != 0 && integralPart >= 10) {
					exponent++;
					integralPart = 1;
				}
			}
		}
	}



	slf_string writeFloat(double value) {
		slf_string strRst;

		if (isnan(value)) return strRst;

		if (value < 0.0) {
			putchar('-');
			value = -value;
		}

		if (isinf(value)) return strRst;

		uint32_t integralPart, decimalPart;
		int16_t exponent;
		splitFloat(value, integralPart, decimalPart, exponent);

		writeInteger(integralPart, strRst);
		if (decimalPart) writeDecimals(decimalPart, strRst);

		if (exponent < 0) {
			strRst.append("e-");
			writeInteger(-exponent, strRst);
		}

		if (exponent > 0) {
			strRst.append('e');
			writeInteger(exponent, strRst);
		}
		return std::move(strRst);
	}





	////////////////////////Decimal System  /////////////////////////////////
	slf_string slf_cast(uint8_t un)
	{
		return unsigned_integer(un);
	}

	slf_string slf_cast(int8_t in)
	{
		return signed_integer(in);
	}

	slf_string slf_cast(uint16_t un)
	{
		return unsigned_integer(un);
	}

	slf_string slf_cast(int16_t in)
	{
		return signed_integer(in);
	}

	slf_string slf_cast(uint32_t un)
	{
		return unsigned_integer(un);
	}

	slf_string slf_cast(int32_t in)
	{
		return signed_integer(in);
	}

	slf_string slf_cast(uint64_t un)
	{
		return unsigned_integer(un);
	}

	slf_string slf_cast(int64_t in)
	{
		return signed_integer(in);
	}

	slf_string slf_cast(double in)
	{
		return writeFloat(in);
	}

	////////////////////////Hexdecimal  /////////////////////////////////
	slf_string slf_cast_hex(uint64_t un)
	{
		return unsigned_integer_hex(un);
	}


	///////////////////////////Float/////////////////////////////////////////
	//slf_string slf_cast_float(double un)
	//{
	//	return writeFloat(un);
	//}
}