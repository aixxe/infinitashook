#pragma once

#include "core/bm2dx/bm2dx_util.h"

namespace infinitashook
{
	class game_environment
	{
		public:
			game_environment(uintptr_t base_addr);

			uintptr_t get_base_addr();
			bm2dx::state_t* get_state();
			bm2dx::judgement_t* get_judgement();
			const char* get_player_name();
			bm2dx::pacemaker_t get_pacemaker_data();
		private:
			uintptr_t _baseAddr = 0;
			bm2dx::state_t* _state = nullptr;
			bm2dx::judgement_t* _judgement = nullptr;
			const char* _playerName = nullptr;
	};
}