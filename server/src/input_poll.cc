#include <vector>
#include <windows.h>
#include "input_poll.h"
#include "util/log.h"

// Basic container for a key.
struct key_t
{
	std::string name;
	uint8_t code;
};

// A list of keys that should be monitored.
std::vector<key_t> keys = {
	{"0", VK_NUMPAD0},
	{"1", VK_NUMPAD1},
	{"2", VK_NUMPAD2},
	{"3", VK_NUMPAD3},
	{"4", VK_NUMPAD4},
	{"5", VK_NUMPAD5},
	{"6", VK_NUMPAD6},
	{"7", VK_NUMPAD7},
	{"8", VK_NUMPAD8},
	{"9", VK_NUMPAD9},
};

namespace infinitashook
{
	bool input_poll::poll()
	{
		bool changed = false;

		for (auto& key: keys)
		{
			bool is_down = GetAsyncKeyState(key.code) != 0;
			bool in_msg = _message.contains(key.name);

			if (in_msg && !is_down)
			{
				_message.erase(key.name);
				changed = true;

				log_debug("%s key is now UP.", key.name.c_str());
			}

			if (!in_msg && is_down)
			{
				_message.push_back(key.name);
				changed = true;

				log_debug("%s key is now DOWN.", key.name.c_str());
			}
		}

		return changed;
	}
	
	input_message input_poll::get_message()
	{
		return _message;
	}
}