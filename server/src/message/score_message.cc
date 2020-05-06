#include <ctime>
#include "score_message.h"

inline std::vector<uint8_t> string_to_bytes(std::string str)
{
	return std::vector<uint8_t>(str.begin(), str.end());
}

namespace infinitashook
{
	score_message::score_message()
	{
		_score["timestamp"] = std::time(0);
	}

	const std::string score_message::get_name()
	{
		return "score";
	}

	const nlohmann::json score_message::get_data()
	{
		return _score;
	}

	void score_message::set_play_style(uint32_t play_style)
	{
		_score["chart"]["play_style"] = play_style;
	}

	void score_message::set_music(bm2dx::music_data* music)
	{
		_score["music"]["id"] = music->entry_id;
		_score["music"]["title"] = string_to_bytes(music->title);
		_score["music"]["artist"] = string_to_bytes(music->artist);
	}

	void score_message::set_chart(bm2dx::chart_t chart, uint32_t difficulty)
	{
		_score["chart"]["rating"] = chart.rating;
		_score["chart"]["difficulty"] = difficulty;
	}

	void score_message::set_player_name(std::string name)
	{
		_score["player"]["name"] = name;
	}

	void score_message::set_clear_type(int32_t best, int32_t current)
	{
		_score["clear"]["best"] = bm2dx::CLEAR_TYPE[best];
		_score["clear"]["current"] = bm2dx::CLEAR_TYPE[current];
	}

	void score_message::set_dj_level(int32_t best, int32_t current)
	{
		_score["dj_level"]["best"] = bm2dx::DJ_LEVEL[best];
		_score["dj_level"]["current"] = bm2dx::DJ_LEVEL[current];
	}

	void score_message::set_ex_score(int32_t best, int32_t current)
	{
		_score["ex_score"]["best"] = best;
		_score["ex_score"]["current"] = current;
	}

	void score_message::set_miss_count(int32_t best, int32_t current)
	{
		_score["miss_count"]["best"] = best;
		_score["miss_count"]["current"] = current;
	}

	void score_message::set_pacemaker(bm2dx::pacemaker_t pacemaker)
	{
		_score["pacemaker"]["name"] = pacemaker.name;
		_score["pacemaker"]["score"] = pacemaker.score;
		_score["pacemaker"]["type"] = bm2dx::PACEMAKER_TYPE[pacemaker.type];
	}

	void score_message::set_judgement(bm2dx::judgement_player_t judgement)
	{
		_score["judgement"]["pgreat"] = judgement.pgreat;
		_score["judgement"]["great"] = judgement.great;
		_score["judgement"]["good"] = judgement.good;
		_score["judgement"]["bad"] = judgement.bad;
		_score["judgement"]["poor"] = judgement.poor;
		_score["judgement"]["combo_break"] = judgement.combo_break;
		_score["judgement"]["fast"] = judgement.fast;
		_score["judgement"]["slow"] = judgement.slow;
	}
}