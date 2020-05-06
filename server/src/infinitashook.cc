#include <windows.h>
#include "context.h"
#include "util/log.h"
#include "event/score_event.h"

using infinitashook::app;

DWORD WINAPI infinitashook_init(LPVOID dll_instance)
{
	app.create(
		reinterpret_cast<uintptr_t>(GetModuleHandleA("bm2dx.exe"))
	);

	auto server = app.get_event_server();
	auto input = app.get_input_poll();
	
	auto result_hook = app.get_result_hook();

	result_hook->install();
	result_hook->add_callback(infinitashook::process_score_event);

	server->bind("tcp://127.0.0.1:5730");

	while (true)
	{
		if (input->poll())
		{
			auto message = input->get_message();
			server->publish(&message);
		}

		if (GetAsyncKeyState(VK_F10))
			break;

		Sleep(100);
	}

	log_info("Shutting down event server..");
	server->close();

	log_info("Freeing resources and ejecting from process..");
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(dll_instance), EXIT_SUCCESS);

	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE dll_instance, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, infinitashook_init, dll_instance, 0, nullptr);

	return TRUE;
}