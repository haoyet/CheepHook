#include "../dependencies/common_includes.hpp"
#include "features/misc/misc.hpp"
#include "menu/config/config.hpp"
#include "features/misc/events.hpp"

unsigned long __stdcall initial_thread(void* reserved) {

#ifdef debug_build
	AllocConsole();
	SetConsoleTitleW(L"Counter-Strike: Global Offensive");
	freopen_s(reinterpret_cast<FILE * *>stdin, "CONIN$", "r", stdin);
	freopen_s(reinterpret_cast<FILE * *>stdout, "CONOUT$", "w", stdout);
#endif

	interfaces::initialize();

	printf("[setup] interfaces initialized!\n");

	hooks::initialize();

	printf("[setup] hooks initialized!\n");

	render.setup_fonts();

	printf("[setup] render initialized!\n");

	utilities::material_setup();

	printf("[setup] materials initialized!\n");

	config_system.run("CheepHook");

	printf("[setup] config initialized!\n");

	events.setup();

	printf("[setup] events initialized!\n");

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	hooks::shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

#ifdef debug_build
	fclose(reinterpret_cast<FILE*>stdin);
	fclose(reinterpret_cast<FILE*>stdout);
	FreeConsole();
#endif

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(reserved), 0);
	return 0ul;
}

bool __stdcall DllMain(void* instance, unsigned long reason_to_call, void* reserved) {
	if (reason_to_call == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, initial_thread, instance, 0, 0);
	}

	return true;
}