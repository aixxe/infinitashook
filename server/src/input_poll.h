#pragma once

#include "message/input_message.h"

namespace infinitashook
{
	class input_poll
	{
		public:
			bool poll();
			input_message get_message();
		private:
			input_message _message;
	};
}