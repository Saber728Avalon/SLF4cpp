#pragma once
#include "config.h"
#include "slf_string.h"
#include "slf_log_provider.h"
#include "slf_format.h"
#include "slf_time_format.h"
#include "slf_enum.h"

namespace SLF4cpp
{
	class slf_main
	{
	public:
		//初始化日志系统
		static int slf4cpp_init(int nType)
		{
			slf_format::InitFormat();
			m_pLogProvider = new slf_log_provider_stdout();

			return 0;
		}

		//设置日志前缀串
		// yyyy-mm-dd HH:mm:ss.SSS level pid
		static int slfcpp_prefix(slf_string::PROTOTYPE *pPrefix)
		{
			return m_timeFormat.analyze(pPrefix);
		}

		//替换日志的框架
		static int replace_log_provider(slf_log_provider *pLogProvider)
		{
			int nRet = 0;
			if (NULL != m_pLogProvider)
			{
				nRet = m_pLogProvider->close();
				if (nRet < 0)
				{
					return nRet;
				}
				delete m_pLogProvider;
			}
			m_pLogProvider = pLogProvider;
		}

		//根据日志登记输出日志
		static void write_log(slf_log_level nLevel,  slf_string &log)
		{
			slf_string strFormatLog = m_timeFormat.format_time(log, nLevel);
#if defined(LOG_END_RETURN)
			strFormatLog.append('\n');
#endif
			if (NULL != m_pLogProvider)
			{
				switch (nLevel)
				{
				case slf_log_level::slf_log_level_debug:
				{
					m_pLogProvider->write_debug(strFormatLog);
				}
				break;

				case slf_log_level::slf_log_level_info:
				{
					m_pLogProvider->write_info(strFormatLog);
				}
				break;

				case slf_log_level::slf_log_level_warn:
				{
					m_pLogProvider->write_warn(strFormatLog);
				}
				break;

				case slf_log_level::slf_log_level_error:
				{
					m_pLogProvider->write_error(strFormatLog);
				}
				break;

				default:
					break;
				}
			}
		}

		static void write_log_func_line(slf_log_level nLevel,  slf_string &log, slf_string &line)
		{
			log = line + log;
			return write_log(nLevel, log);
		}

	private:
		static slf_log_provider *m_pLogProvider;
		static slf_time_foramt m_timeFormat;
	};

	slf_log_provider* slf_main::m_pLogProvider = NULL;
	slf_time_foramt slf_main::m_timeFormat;
}

