#pragma once
#include "slf_string.h"
#include "slf_log_provider.h"
#include "slf_format.h"

namespace SLF4cpp
{
	enum slf_log_level
	{
		slf_log_level_debug,
		slf_log_level_info,
		slf_log_level_warn,
		slf_log_level_error,

		slf_log_level_count
	};

	class slf_main
	{
	public:
		static int slf4cpp_init(int nType)
		{
			slf_format::InitFormat();
			m_pLogProvider = new slf_log_provider_stdout();

			return 0;
		}

		// yyyy-mm-dd HH:mm:ss.SSS level pid
		static int slfcpp_prefix(slf_string::PROTOTYPE *pPrefix)
		{

		}

		//
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
				delete[] m_pLogProvider;
			}
			m_pLogProvider = pLogProvider;
		}

		static void write_log(slf_log_level nLevel,  const slf_string &log)
		{
			if (NULL != m_pLogProvider)
			{
				switch (nLevel)
				{
				case slf_log_level::slf_log_level_debug:
				{
					m_pLogProvider->write_debug(log);
				}
				break;

				case slf_log_level::slf_log_level_info:
				{
					m_pLogProvider->write_info(log);
				}
				break;

				case slf_log_level::slf_log_level_warn:
				{
					m_pLogProvider->write_warn(log);
				}
				break;

				case slf_log_level::slf_log_level_error:
				{
					m_pLogProvider->write_error(log);
				}
				break;

				default:
					break;
				}
			}
		}

	private:
		static slf_log_provider *m_pLogProvider;
	};

	slf_log_provider* slf_main::m_pLogProvider = NULL;
}

