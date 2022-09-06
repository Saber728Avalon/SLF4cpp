#include "slf4cpp.h"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"

namespace SLF4cpp
{
	class slf_log_provider_plog : public slf_log_provider
	{
	public:
		~slf_log_provider_plog();

		virtual int init()
		{
			 plog::init(plog::debug, "Hello.txt");
			 return 0;
		}

		virtual int write_debug(const slf_string &asciiString)
		{
			PLOG(plog::debug) << asciiString.c_str();
			return 0;
		}
		virtual int write_info(const slf_string &asciiString)
		{
			PLOG(plog::info) << asciiString.c_str();
			return 0;
		}
		virtual int write_warn(const slf_string &asciiString)
		{
		PLOG(plog::warning) << asciiString.c_str();
			return 0;
		}
		virtual int write_error(const slf_string &asciiString)
		{
		PLOG(plog::error) << asciiString.c_str();
			return 0;
		}
		virtual int close() 
		{
			return 0;
		}
	};
}