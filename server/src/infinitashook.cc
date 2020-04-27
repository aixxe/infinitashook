#include <cstdint>
#include <iostream>
#include <windows.h>
#include <MinHook.h>

#include "bm2dx.h"
#include "bm2dx_util.h"

std::uintptr_t bm2dx_addr = 0;      // Base address of the 'bm2dx.exe' module.
state_t* state = nullptr;           // Structure containing various game state data.
judgement_t* judgement = nullptr;   // Structure containing judgement data for both players.
const char* player_name = nullptr;  // DJ name of the player, works for both sides.
void* get_pacemaker_data = nullptr; // A function for retrieving pacemaker data.

void* result_hook_original_fn = nullptr;
void* result_hook_return_addr = nullptr;

// Print score data out to the console. Just a placeholder for now.
void scorehook_dump(StageResultDrawFrame* frame = nullptr) {
    auto chart = bm2dx::get_chart();
    auto judgement = bm2dx::get_judgement();

    printf("Player name:        %s\n\n", player_name);

    printf("Active play side:   %s\n", state->p1_active ? "P1" : "P2");
    printf("Active play style:  %s\n\n", state->play_style == 0 ? "Single" : "Double");

    printf("Current music:      %s - %s\n", state->music->artist, state->music->title);

    printf("Chart note count:   %i\n", chart.notes);
    printf("Chart rating:       Lv. %i\n", chart.rating);

    printf("Chart BPM:          %i ~ %i\n\n", chart.bpm_min, chart.bpm_max);

    printf("PGREAT:             %i\n", judgement.pgreat);
    printf("GREAT:              %i\n", judgement.great);
    printf("GOOD:               %i\n", judgement.good);
    printf("BAD:                %i\n", judgement.bad);
    printf("POOR:               %i\n\n", judgement.poor);

    printf("COMBO BREAK:        %i\n\n", judgement.combo_break);

    printf("FAST:               %i\n", judgement.fast);
    printf("SLOW:               %i\n\n", judgement.slow);

    auto frame_data = state->p1_active ? frame->p1: frame->p2;

    printf("Best clear type:    %s\n", CLEAR_TYPE[frame_data.best_clear_type]);
    printf("Current clear type: %s\n\n", CLEAR_TYPE[frame_data.current_clear_type]);

    printf("Best DJ level:      %s\n", DJ_LEVEL[frame_data.best_dj_level]);
    printf("Current DJ level:   %s\n\n", DJ_LEVEL[frame_data.current_dj_level]);

    printf("Best EX score:      %i\n", frame_data.best_ex_score);
    printf("Current EX score:   %i\n\n", frame_data.current_ex_score);

    printf("Best miss count:    %i\n", frame_data.best_miss_count);
    printf("Current miss count: %i\n\n", frame_data.current_miss_count);

    pacemaker_t pacemaker_data;

    _asm {
        lea eax, [pacemaker_data];
        call[get_pacemaker_data];
    }

    printf("Pacemaker target:   %i [%s]\n", pacemaker_data.score, pacemaker_data.name);
    printf("Pacemaker type:     %s\n", PACEMAKER_TYPE[pacemaker_data.type]);
}

__declspec(naked) void scorehook_intercept() {
    __asm {
        call        [result_hook_original_fn];
        cmp         ecx, 5;
        jne         back;
        pushad;
        push        eax;
        call        [scorehook_dump];
        pop         eax;
        popad;
    back:
        jmp[result_hook_return_addr];
    }
}

DWORD WINAPI scorehook_init(LPVOID dll_instance) {
    // Create a console window for printing text.
    AllocConsole();
    freopen_s((FILE**) stdout, "CONOUT$", "w", stdout);

    // Get the base address of bm2dx.exe.
    bm2dx_addr = (std::uintptr_t) GetModuleHandleA("bm2dx.exe");

    // Cast various interesting areas of game memory.
    state = (state_t*)(bm2dx_addr + game_state_addr);
    judgement = (judgement_t*)(bm2dx_addr + judgement_addr);
    player_name = (const char*)(bm2dx_addr + player_name_addr);
    get_pacemaker_data = (void*)(bm2dx_addr + pacemaker_addr);

    MH_Initialize();

    const std::uintptr_t hook_address = (bm2dx_addr + result_hook_addr);

    result_hook_original_fn = GetAbsoluteAddress(hook_address);
    result_hook_return_addr = (void*)(hook_address + 5);

    MH_CreateHook((void*)hook_address, &scorehook_intercept, NULL);
    MH_EnableHook((void*)hook_address);

    do {
        if (GetAsyncKeyState(VK_F10))
            break;

        Sleep(100);
    } while (true);

    // Print some text, just so we know that something is happening.
    printf("Detaching from process..\n");

    // Free resources and detach from process.
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)dll_instance, EXIT_SUCCESS);

    return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE dll_instance, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(NULL, NULL, scorehook_init, dll_instance, NULL, NULL);

    return TRUE;
}