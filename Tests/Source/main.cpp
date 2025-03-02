
#define ENABLE_ASSERT 1
#define ENABLE_VERIFY 1

#include "MElib/MElib.h"
#include <spdlog/spdlog.h>
#include <format>

namespace MElib {

	void PrintAssertMessage(std::string_view prefix, std::string_view condition, std::string_view message, const std::source_location& location)
	{
		spdlog::error("{} {} {}:{}:{}:{}", prefix, message.empty() ? condition : message, location.file_name(), location.line(), location.column(), location.function_name());
	}

}

using namespace MElib::Types;

int main()
{
}
