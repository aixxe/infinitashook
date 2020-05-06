#include "score_event.h"
#include "../message/score_message.h"

namespace infinitashook
{
	void process_score_event(bm2dx::StageResultDrawFrame* frame)
	{
		score_message message;

		auto game = app.get_game_environment();
		auto state = game->get_state();
		auto player_name = game->get_player_name();
		auto difficulty = bm2dx::get_difficulty(state);
		auto chart = bm2dx::get_chart(state, difficulty);
		auto judgement = bm2dx::get_judgement(state, game->get_judgement());
		auto frame_data = (state->p1_active) ? frame->p1: frame->p2;
		auto pacemaker = game->get_pacemaker_data();

		message.set_play_style(state->play_style);
		message.set_music(state->music);
		message.set_chart(chart, difficulty);
		message.set_player_name(player_name);
		message.set_clear_type(frame_data.best_clear_type, frame_data.current_clear_type);
		message.set_dj_level(frame_data.best_dj_level, frame_data.current_dj_level);
		message.set_ex_score(frame_data.best_ex_score, frame_data.current_ex_score);
		message.set_miss_count(frame_data.best_miss_count, frame_data.current_miss_count);
		message.set_pacemaker(pacemaker);
		message.set_judgement(judgement);

		app.get_event_server()->publish(&message);
	}
}