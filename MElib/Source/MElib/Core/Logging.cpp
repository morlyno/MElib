#include "Logging.h"

#include <spdlog/spdlog.h>

namespace MElib {

}

namespace MElib::Logging {

	void SetLogLevelTrace()
	{
		spdlog::set_level(spdlog::level::trace);
	}

	void Trace(std::string_view message)
	{
		spdlog::trace(message);
	}

	void Debug(std::string_view message)
	{
		spdlog::debug(message);
	}

	void Info(std::string_view message)
	{
		spdlog::info(message);
	}

	void Warn(std::string_view message)
	{
		spdlog::warn(message);
	}

	void Error(std::string_view message)
	{
		spdlog::error(message);
	}

	void Critical(std::string_view message)
	{
		spdlog::critical(message);
	}

	void PrintAssertMessage(std::string_view message)
	{
		spdlog::error(message);
	}

}

