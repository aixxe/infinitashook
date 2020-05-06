#pragma once

#include <nlohmann/json.hpp>

namespace infinitashook
{
	class base_message
	{
		public:
			virtual const std::string get_name() = 0;
			virtual const nlohmann::json get_data() = 0;

			const std::string dump();
	};
}