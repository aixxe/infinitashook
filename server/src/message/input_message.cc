#include "input_message.h"

namespace infinitashook
{
	const std::string input_message::get_name()
	{
		return "input";
	}

	const nlohmann::json input_message::get_data()
	{
		return {
			{"keys", this->_keys},
		};
	}

	void input_message::push_back(std::string key)
	{
		if (_keys.count(key) == 0)
		{
			_keys.insert(key);
		}
	}

	bool input_message::contains(std::string key)
	{
		return (_keys.count(key) == 1);
	}

	void input_message::erase(std::string key)
	{
		_keys.erase(key);
	}
}