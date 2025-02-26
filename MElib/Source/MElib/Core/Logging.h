#pragma once

#include "MElib/std/Include.h"

namespace MElib::Logging {

	void Trace(std::string_view message);
	void Debug(std::string_view message);
	void Info(std::string_view message);
	void Warn(std::string_view message);
	void Error(std::string_view message);
	void Critical(std::string_view message);

	template<typename... TArgs>
	void PrintAssertMessage(std::string_view prefix, std::format_string<TArgs...> format, TArgs&&... args);
	void PrintAssertMessage(std::string_view message);

}

template<typename... TArgs>
void MElib::Logging::PrintAssertMessage(std::string_view prefix, std::format_string<TArgs...> fmt, TArgs&&... args)
{
	PrintAssertMessage(std::format("{}: {}", prefix, std::format(fmt, std::forward<TArgs>(args)...)));
}
