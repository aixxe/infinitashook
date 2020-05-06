#include "game_environment.h"

void* _getPacemakerData = nullptr;

namespace infinitashook
{
	game_environment::game_environment(uintptr_t base_addr)
	{
		_baseAddr = base_addr;
		_state = reinterpret_cast<decltype(_state)>(base_addr + bm2dx::game_state_addr);
		_judgement = reinterpret_cast<decltype(_judgement)>(base_addr + bm2dx::judgement_addr);
		_playerName = reinterpret_cast<decltype(_playerName)>(base_addr + bm2dx::player_name_addr);
		_getPacemakerData = reinterpret_cast<decltype(_getPacemakerData)>(base_addr + bm2dx::pacemaker_addr);
	}

	uintptr_t game_environment::get_base_addr()
	{
		return _baseAddr;
	}

	bm2dx::state_t* game_environment::get_state()
	{
		return _state;
	}

	bm2dx::judgement_t* game_environment::get_judgement()
	{
		return _judgement;
	}

	const char* game_environment::get_player_name()
	{
		return _playerName;
	}

	bm2dx::pacemaker_t game_environment::get_pacemaker_data()
	{
		bm2dx::pacemaker_t pacemaker_data {};

		#ifdef __GNUC__
			asm volatile ("leal %0, %%eax\n" :: "m" (pacemaker_data));
			asm volatile ("call *__getPacemakerData");
		#else
			_asm
			{
				lea eax, [pacemaker_data];
				call [_getPacemakerData];
			}
		#endif

		return pacemaker_data;
	}
}
