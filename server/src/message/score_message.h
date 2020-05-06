#pragma once

#include "base_message.h"
#include "../core/bm2dx/bm2dx_util.h"

namespace infinitashook
{
	class score_message: public base_message
	{
		public:
			score_message();

			const std::string get_name() override;
			const nlohmann::json get_data() override;

			void set_play_style(uint32_t play_style);
			void set_music(bm2dx::music_data* music);
			void set_chart(bm2dx::chart_t chart, uint32_t difficulty);
			void set_player_name(std::string name);
			void set_clear_type(int32_t best, int32_t current);
			void set_dj_level(int32_t best, int32_t current);
			void set_ex_score(int32_t best, int32_t current);
			void set_miss_count(int32_t best, int32_t current);
			void set_pacemaker(bm2dx::pacemaker_t pacemaker);
			void set_judgement(bm2dx::judgement_player_t judgement);
		private:
			nlohmann::json _score;
	};
}