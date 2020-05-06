#include "base_message.h"

namespace infinitashook
{
	const std::string base_message::dump()
	{
		auto name = get_name();
		auto data = get_data();

		return nlohmann::json({
			{"event", name},
			{"data", data}
		}).dump();
	}
}