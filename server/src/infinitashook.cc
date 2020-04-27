#include <cstdint>
#include <iostream>
#include <windows.h>
#include <MinHook.h>
#include <zmq.hpp>
#include <nlohmann/json.hpp>

#include "bm2dx.h"
#include "bm2dx_util.h"

std::uintptr_t bm2dx_addr = 0;      // Base address of the 'bm2dx.exe' module.
state_t* state = nullptr;           // Structure containing various game state data.
judgement_t* judgement = nullptr;   // Structure containing judgement data for both players.
const char* player_name = nullptr;  // DJ name of the player, works for both sides.
void* get_pacemaker_data = nullptr; // A function for retrieving pacemaker data.

void* result_hook_original_fn = nullptr;
void* result_hook_return_addr = nullptr;

zmq::context_t context;
zmq::socket_t server;

void scorehook_dump(StageResultDrawFrame* frame = nullptr) {
    nlohmann::json message;

    auto chart = bm2dx::get_chart();
    auto judgement = bm2dx::get_judgement();
    auto frame_data = state->p1_active ? frame->p1: frame->p2;

    pacemaker_t pacemaker_data;

    _asm {
        lea eax, [pacemaker_data];
        call[get_pacemaker_data];
    }

    message["player"]["name"] = player_name;
    
    message["music"]["id"] = state->music->entry_id;
    message["music"]["title"] = state->music->title;
    message["music"]["artist"] = state->music->artist;

    message["chart"]["rating"] = chart.rating;
    message["chart"]["play_style"] = state->play_style;
    message["chart"]["difficulty"] = state->p1_active ? state->p1_difficulty: state->p2_difficulty;

    message["judgement"]["pgreat"] = judgement.pgreat;
    message["judgement"]["great"] = judgement.great;
    message["judgement"]["good"] = judgement.good;
    message["judgement"]["bad"] = judgement.bad;
    message["judgement"]["poor"] = judgement.poor;

    message["judgement"]["combo_break"] = judgement.combo_break;

    message["judgement"]["fast"] = judgement.fast;
    message["judgement"]["slow"] = judgement.slow;

    message["clear"]["best"] = CLEAR_TYPE[frame_data.best_clear_type];
    message["clear"]["current"] = CLEAR_TYPE[frame_data.current_clear_type];

    message["dj_level"]["best"] = DJ_LEVEL[frame_data.best_dj_level];
    message["dj_level"]["current"] = DJ_LEVEL[frame_data.current_dj_level];

    message["ex_score"]["best"] = frame_data.best_ex_score;
    message["ex_score"]["current"] = frame_data.current_ex_score;

    message["miss_count"]["best"] = frame_data.best_miss_count;
    message["miss_count"]["current"] = frame_data.current_miss_count;

    message["pacemaker"]["name"] = pacemaker_data.name;
    message["pacemaker"]["score"] = pacemaker_data.score;
    message["pacemaker"]["type"] = PACEMAKER_TYPE[pacemaker_data.type];

    const auto str = message.dump();

    server.send(zmq::message_t { str.c_str(), str.size() }, zmq::send_flags::none);
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

    // Initialise and bind the server.
    server = zmq::socket_t(context, zmq::socket_type::pub);
    server.bind("tcp://0.0.0.0:5730");

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

    // Need to close these manually for detaching to work properly.
    server.close();
    context.close();

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