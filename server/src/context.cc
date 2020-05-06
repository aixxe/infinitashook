#include <MinHook.h>
#include "context.h"

namespace infinitashook
{
	context app;
	
	context::context()
	{
		AllocConsole();
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

		MH_Initialize();
	}

	context::~context()
	{
		FreeConsole();

		MH_Uninitialize();
	}

	void context::create(uintptr_t base_addr)
	{
		_gameEnvironment = std::make_shared<game_environment>(base_addr);
		_eventServer = std::make_shared<event_server>();
		_inputPoll = std::make_shared<input_poll>();
		_resultHook = std::make_shared<result_hook>();
	}

	std::shared_ptr<game_environment> context::get_game_environment()
	{
		return _gameEnvironment;
	}

	std::shared_ptr<event_server> context::get_event_server()
	{
		return _eventServer;
	}

	std::shared_ptr<input_poll> context::get_input_poll()
	{
		return _inputPoll;
	}

	std::shared_ptr<result_hook> context::get_result_hook()
	{
		return _resultHook;
	}
}