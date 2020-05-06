#include "bm2dx_util.h"

namespace bm2dx
{
	const judgement_player_t get_judgement(state_t* state, judgement_t* judgement)
	{
		if (state->play_style == PLAYSTYLE_DP)
		{
			return {
				judgement->p1_pgreat + judgement->p2_pgreat,
				judgement->p1_great + judgement->p2_great,
				judgement->p1_good + judgement->p2_good,
				judgement->p1_bad + judgement->p2_bad,
				judgement->p1_poor + judgement->p2_poor,
				judgement->p1_combo_break + judgement->p2_combo_break,
				judgement->p1_fast + judgement->p2_fast,
				judgement->p1_slow + judgement->p2_slow,
			};
		}

		return {
			state->p1_active ? judgement->p1_pgreat: judgement->p2_pgreat,
			state->p1_active ? judgement->p1_great: judgement->p2_great,
			state->p1_active ? judgement->p1_good: judgement->p2_good,
			state->p1_active ? judgement->p1_bad: judgement->p2_bad,
			state->p1_active ? judgement->p1_poor: judgement->p2_poor,
			state->p1_active ? judgement->p1_combo_break: judgement->p2_combo_break,
			state->p1_active ? judgement->p1_fast: judgement->p2_fast,
			state->p1_active ? judgement->p1_slow: judgement->p2_slow,
		};
	}

	// Single play charts.
	const chart_t get_spn_chart(music_data* music)
	{
		return {
			music->sp_normal_rating,
			music->sp_normal_bpm_min,
			music->sp_normal_bpm_max,
			music->sp_normal_notes
		};
	}

	const chart_t get_sph_chart(music_data* music)
	{
		return {
			music->sp_hyper_rating,
			music->sp_hyper_bpm_min,
			music->sp_hyper_bpm_max,
			music->sp_hyper_notes
		};
	}

	const chart_t get_spa_chart(music_data* music)
	{
		return {
			music->sp_another_rating,
			music->sp_another_bpm_min,
			music->sp_another_bpm_max,
			music->sp_another_notes
		};
	}

	// Double play charts.
	const chart_t get_dpn_chart(music_data* music)
	{
		return {
			music->dp_normal_rating,
			music->dp_normal_bpm_min,
			music->dp_normal_bpm_max,
			music->dp_normal_notes
		};
	}

	const chart_t get_dph_chart(music_data* music)
	{
		return {
			music->dp_hyper_rating,
			music->dp_hyper_bpm_min,
			music->dp_hyper_bpm_max,
			music->dp_hyper_notes
		};
	}

	const chart_t get_dpa_chart(music_data* music)
	{
		return {
			music->dp_another_rating,
			music->dp_another_bpm_min,
			music->dp_another_bpm_max,
			music->dp_another_notes
		};
	}

	const uint32_t get_difficulty(state_t* state)
	{
		return state->p1_active ? state->p1_difficulty: state->p2_difficulty;
	}

	const chart_t get_chart(state_t* state, uint32_t difficulty)
	{
		switch (difficulty)
		{
			case DIFFICULTY_NORMAL:
				return (state->play_style == PLAYSTYLE_SP) ?
					get_spn_chart(state->music): get_dpn_chart(state->music);
			case DIFFICULTY_HYPER:
				return (state->play_style == PLAYSTYLE_SP) ?
					get_sph_chart(state->music): get_dph_chart(state->music);
			default:
				return (state->play_style == PLAYSTYLE_SP) ?
					get_spa_chart(state->music): get_dpa_chart(state->music);
		}
	}
}