#include <MinHook.h>
#include "result_hook.h"
#include "../context.h"
#include "../util/log.h"

void* original_fn = nullptr;
void* return_addr = nullptr;

template <typename T = void*> inline T resolve_address(std::uintptr_t addr, std::size_t offset = 0, std::size_t length = 5)
{
	return reinterpret_cast<T>(addr + length + *reinterpret_cast<std::uint32_t*>(addr + 1) + offset);
}

extern "C" void invoke_callbacks(bm2dx::StageResultDrawFrame* frame)
{
	log_debug("Got StageResultDrawFrame at 0x%X, invoking callbacks..", uintptr_t(frame));
	infinitashook::app.get_result_hook()->invoke_callbacks(frame);
}

#ifdef __GNUC__
	extern "C" void result_hook_wrapper(void);
	__asm__
	(
		"_result_hook_wrapper:\n"
			"  call *_original_fn\n"
			"  cmp $5, %ecx\n"
			"  jne back\n"
			"  pushal\n"
			"  push %eax\n"
			"  call _invoke_callbacks\n"
			"  pop %eax\n"
			"  popal\n"
		"back:\n"
			"  jmp *_return_addr\n"
	);
#else
	__declspec(naked) void result_hook_wrapper()
	{
		__asm
		{
			call        [original_fn];
			cmp         ecx, 5;
			jne         back;
			pushad;
			push        eax;
			call        [invoke_callbacks];
			pop         eax;
			popad;
		back:
			jmp         [return_addr];
		}
	}
#endif

namespace infinitashook
{
	void result_hook::install()
	{
		const std::uintptr_t hook_address =
			(app.get_game_environment()->get_base_addr() + bm2dx::result_hook_addr);

		original_fn = resolve_address(hook_address);
		return_addr = reinterpret_cast<void*>(hook_address + 5);

		log_debug("Installing result hook at 0x%X.", hook_address);

		MH_CreateHook(reinterpret_cast<void*>(hook_address), reinterpret_cast<void*>(&result_hook_wrapper), nullptr);
		MH_EnableHook(reinterpret_cast<void*>(hook_address));
	}
}