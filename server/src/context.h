#pragma once

#include <memory>

#include "core/event_server.h"
#include "hook/result_hook.h"
#include "game_environment.h"
#include "input_poll.h"

namespace infinitashook
{
	class context
	{
		public:
			context();
			~context();

			void create(uintptr_t base_addr);

			std::shared_ptr<game_environment> get_game_environment();
			std::shared_ptr<event_server> get_event_server();
			std::shared_ptr<input_poll> get_input_poll();
			std::shared_ptr<result_hook> get_result_hook();
		private:
			std::shared_ptr<game_environment> _gameEnvironment;
			std::shared_ptr<event_server> _eventServer;
			std::shared_ptr<input_poll> _inputPoll;
			std::shared_ptr<result_hook> _resultHook;
	};

	extern context app;
}