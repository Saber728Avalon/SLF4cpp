#include "slf4cpp.h"
#include "easyloggingpp/easylogging++.h"

namespace SLF4cpp
{
	class slf_log_provider_plog : public slf_log_provider
	{
	public:
		~slf_log_provider_plog();

		virtual int init()
		{
			el::Configurations defaultConf;
			defaultConf.setToDefault();
			 // Values are always std::string
			defaultConf.set(el::Level::Info,
			         el::ConfigurationType::Format, "%datetime %level %msg");
			 // default logger uses default configurations
			 el::Loggers::reconfigureLogger("default", defaultConf);
			 LOG(INFO) << "Log using default file";
			 // To set GLOBAL configurations you may use
			defaultConf.setGlobally(
					 el::ConfigurationType::Format, "%date %msg");
			el::Loggers::reconfigureLogger("default", defaultConf);
			 return 0;
		}

		virtual int write_debug(const slf_string &asciiString)
		{
			LOG(INFO) << asciiString.c_str();
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