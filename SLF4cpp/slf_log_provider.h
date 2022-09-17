#pragma once
#include "slf_string.h"


namespace SLF4cpp
{
	//日志输出的基类。实现新的日志支持，只需要继承这个类
	class slf_log_provider
	{
	public:
		~slf_log_provider() {}

		//初始化日志框架
		virtual int init() = 0;
		
		//根据不同等级,日志输出
		virtual int write_debug(const slf_string &asciiString) = 0;
		virtual int write_info(const slf_string &asciiString) = 0;
		virtual int write_warn(const slf_string &asciiString) = 0;
		virtual int write_error(const slf_string &asciiString) = 0;

		//关闭日志
		virtual int close() = 0;
	};

	//标准日志输出
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

	//文件日志输出
	class slf_log_provider_file : public slf_log_provider
	{
	public:
		virtual int init()
		{
			m_pFile = fopen("xxx.log", "ab+");
			return 0;
		}
		virtual int write_debug(const slf_string &asciiString)
		{
			fwrite(asciiString.c_str(), asciiString.length(),1, m_pFile);
			return 0;
		}
		virtual int write_info(const slf_string &asciiString)
		{
			fwrite(asciiString.c_str(), asciiString.length(),1, m_pFile);
			return 0;
		}
		virtual int write_warn(const slf_string &asciiString)
		{
			fwrite(asciiString.c_str(), asciiString.length(),1, m_pFile);
			return 0;
		}
		virtual int write_error(const slf_string &asciiString)
		{
			fwrite(asciiString.c_str(), asciiString.length(),1, m_pFile);
			return 0;
		}

		virtual int close()
		{
			return 0;
		}
	private:
		static FILE *m_pFile;
	};
	FILE* slf_log_provider_file::m_pFile = NULL;
}


