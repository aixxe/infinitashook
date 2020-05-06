#pragma once

#include "bm2dx.h"

namespace bm2dx {
	struct chart_t
	{
		int8_t rating;
		int32_t bpm_min;
		int32_t bpm_max;
		uint32_t notes;
	};

	struct judgement_player_t
	{
		int32_t pgreat;
		int32_t great;
		int32_t good;
		int32_t bad;
		int32_t poor;
		int32_t combo_break;
		int32_t fast;
		int32_t slow;
	};

	const judgement_player_t get_judgement(state_t* state, judgement_t* judgement);

	// Single play charts.
	const chart_t get_spn_chart(music_data* music);
	const chart_t get_sph_chart(music_data* music);
	const chart_t get_spa_chart(music_data* music);

	// Double play charts.
	const chart_t get_dpn_chart(music_data* music);
	const chart_t get_dph_chart(music_data* music);
	const chart_t get_dpa_chart(music_data* music);

	const uint32_t get_difficulty(state_t* state);
	const chart_t get_chart(state_t* state, uint32_t difficulty);
}