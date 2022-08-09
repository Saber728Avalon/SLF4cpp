﻿#pragma once
#include "slf_string.h"


namespace SLF4cpp
{
	class slf_log_provider
	{
	public:
		~slf_log_provider() {}

		virtual int init() = 0;
		virtual int write_debug(const slf_string &asciiString) = 0;
		virtual int write_info(const slf_string &asciiString) = 0;
		virtual int write_warn(const slf_string &asciiString) = 0;
		virtual int write_error(const slf_string &asciiString) = 0;

		virtual int close() = 0;
	};

	class slf_log_provider_stdout : public slf_log_provider
	{
	public:
		virtual int init()
		{
			return 0;
		}
		virtual int write_debug(const slf_string &asciiString)
		{
			printf("%s", asciiString.c_str());
			return 0;
		}
		virtual int write_info(const slf_string &asciiString)
		{
			printf("%s", asciiString.c_str());
			return 0;
		}
		virtual int write_warn(const slf_string &asciiString)
		{
			printf("%s", asciiString.c_str());
			return 0;
		}
		virtual int write_error(const slf_string &asciiString)
		{
			printf("%s", asciiString.c_str());
			return 0;
		}

		virtual int close()
		{
			return 0;
		}
	};
}

