#pragma once

#include <set>
#include "base_message.h"

namespace infinitashook
{
	class input_message: public base_message
	{
		public:
			const std::string get_name() override;
			const nlohmann::json get_data() override;

			void push_back(std::string key);
			bool contains(std::string key);
			void erase(std::string key);
		private:
			std::set<std::string> _keys;
	};
}