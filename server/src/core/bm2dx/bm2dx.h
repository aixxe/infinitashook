#pragma once

/*
 * bm2dx.exe
 *
 * Size: 9065312 bytes (8852 KiB)
 * SHA256: 68BBE6E187D92AB1DF407FB8773FFB19CBA05E844E21503BDB62C9A479D299B7
 */

#include <cstdint>
#include <cstddef>

namespace bm2dx
{
	// Offsets
	constexpr std::uintptr_t game_state_addr = 0x267126C;
	constexpr std::uintptr_t judgement_addr = 0x28EE0DC;
	constexpr std::uintptr_t pacemaker_addr = 0x128DC0;
	constexpr std::uintptr_t player_name_addr = 0x10F036D;
	constexpr std::uintptr_t result_hook_addr = 0x12D6E8;

	// Constants
	constexpr auto PLAYER_P1 = 0;
	constexpr auto PLAYER_P2 = 1;

	constexpr auto PLAYSTYLE_SP = 0;
	constexpr auto PLAYSTYLE_DP = 1;

	constexpr auto DIFFICULTY_NORMAL = 0;
	constexpr auto DIFFICULTY_HYPER = 1;
	constexpr auto DIFFICULTY_ANOTHER = 2;

	// String tables
	extern const char* DJ_LEVEL[];
	extern const char* PACEMAKER_TYPE[];
	extern const char* CLEAR_TYPE[];

	// Classes
	class StageResultDrawFrame
	{
		private:
			virtual ~StageResultDrawFrame() = 0;
		public:
			struct frame_player_t
			{
				int32_t best_clear_type; //0x0004
				int32_t current_clear_type; //0x0008
				int32_t best_dj_level; //0x000C
				int32_t current_dj_level; //0x0010
				int32_t best_ex_score; //0x0014
				int32_t current_ex_score; //0x0018
				int32_t best_miss_count; //0x001C
				int32_t current_miss_count; //0x0020
				int8_t show_best_clear_type; //0x0024
				int8_t show_current_clear_type; //0x0025
				int8_t show_best_dj_level; //0x0026
				int8_t show_current_dj_level; //0x0027
				int8_t show_best_ex_score; //0x0028
				int8_t show_current_ex_score; //0x0029
				int8_t show_best_miss_count; //0x002A
				int8_t show_current_miss_count; //0x002B
				int32_t highlighted_clear_type; //0x002C
				int32_t highlighted_dj_level; //0x0030
				int32_t highlighted_ex_score; //0x0034
				int32_t highlighted_miss_count; //0x0038
			} p1, p2;
	};

	// Structures
	struct music_data
	{
		char title[64]; //0x0000
		char title_ascii[64]; //0x0040
		char genre[64]; //0x0080
		char artist[64]; //0x00C0
		char pad_0100[32]; //0x0100
		int8_t sp_normal_rating; //0x0120
		int8_t sp_hyper_rating; //0x0121
		int8_t sp_another_rating; //0x0122
		int8_t dp_normal_rating; //0x0123
		int8_t dp_hyper_rating; //0x0124
		int8_t dp_another_rating; //0x0125
		char pad_0126[2]; //0x0126
		int32_t sp_normal_bpm_max; //0x0128
		int32_t sp_normal_bpm_min; //0x012C
		int32_t sp_hyper_bpm_max; //0x0130
		int32_t sp_hyper_bpm_min; //0x0134
		int32_t sp_another_bpm_max; //0x0138
		int32_t sp_another_bpm_min; //0x013C
		int32_t dp_normal_bpm_max; //0x0140
		int32_t dp_normal_bpm_min; //0x0144
		int32_t dp_hyper_bpm_max; //0x0148
		int32_t dp_hyper_bpm_min; //0x014C
		int32_t dp_another_bpm_max; //0x0150
		int32_t dp_another_bpm_min; //0x0154
		char pad_0158[16]; //0x0158
		uint32_t sp_normal_notes; //0x0168
		uint32_t sp_hyper_notes; //0x016C
		uint32_t sp_another_notes; //0x0170
		uint32_t dp_normal_notes; //0x0174
		uint32_t dp_hyper_notes; //0x0178
		uint32_t dp_another_notes; //0x017C
		char pad_0180[72]; //0x0180
		int32_t entry_id; //0x01C8
		char pad_01CC[340]; //0x01CC
	};

	struct state_t
	{
		uint32_t play_style;
		char pad_0004[4];
		uint32_t p1_difficulty;
		uint32_t p2_difficulty;
		uint32_t p1_active;
		uint32_t p2_active;
		music_data* music;
	};

	struct pacemaker_t
	{
		int32_t type;
		int32_t score;
		char name[256];
	};

	struct judgement_t
	{
		int32_t p1_pgreat;
		int32_t p1_great;
		int32_t p1_good;
		int32_t p1_bad;
		int32_t p1_poor;
		int32_t p2_pgreat;
		int32_t p2_great;
		int32_t p2_good;
		int32_t p2_bad;
		int32_t p2_poor;
		int32_t p1_combo_break;
		int32_t p2_combo_break;
		int32_t p1_fast;
		int32_t p2_fast;
		int32_t p1_slow;
		int32_t p2_slow;
	};
}