// redo the hook shit for um you know delta buffer
#include "hooks.h"
#include "minhook/minhook.h"

#include "../config/config.h"
#include "../globals/globals.h"
#include "../modules/modules.h"
#include "../render_manager/render_manager.h"
#include "../game_utilities/game_utilities.h"
#include "../pattern_list/pattern_list.h"
#include "../ui/ui.h"

#include "../bonus_features/miscellaneous/miscellaneous.h"
#include "../bonus_features/triggerbot/triggerbot.h"
#include "../bonus_features/speedhack/speedhack.h"
#include "../bonus_features/knifebot/knifebot.h"
#include "../bonus_features/visuals/visuals.h"
#include "../bonus_features/esp/esp.h"
#include "../bonus_features/aimbot/aimbot.h"

#include "../../utils/utils.h"

#include <mutex>

hooks g_hooks;

using present_fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
using reset_fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using create_move_fn = bool(__fastcall*)(void* thisptr, void* edx, float input_sample_frametime, c_user_cmd* cmd);
using get_tick_count_fn = DWORD(WINAPI*)();
using get_tick_count64_fn = ULONGLONG(WINAPI*)();
using time_get_time_fn = DWORD(*)();
using query_performance_counter_fn = BOOL(WINAPI*)(LARGE_INTEGER*);

present_fn                                         present_original = {};
reset_fn                                           reset_original = {};
create_move_fn                                     create_move_original = {};
get_tick_count_fn                                  get_tick_count_original = {};
get_tick_count64_fn                                get_tick_count64_original = {};
time_get_time_fn                                   time_get_time_original = {};
query_performance_counter_fn                       query_performance_counter_original = {};

std::once_flag                                     m_flg = {};
float                                              m_speed = 1.0f;

long __stdcall present_h(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	std::call_once(m_flg, [&] {
		g_render.initialize(device);
		g_ui.initialize(device);
	});

	g_render.begin_render_states(device);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	g_esp.run();
	g_visuals.run();
	g_ui.run();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	g_render.end_render_states(device);

	return present_original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	g_render.undo();
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = reset_original(device, present_parameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	g_render.create_objects();

	return result;
}

bool __fastcall create_move_h(void* thisptr, void* edx, float ft, c_user_cmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return create_move_original(thisptr, edx, ft, cmd);

	/*auto local = g.get_local();
	if (local) {
		tickbase.on_create_move(cmd, local);
	}
	*/
	g_misc.run(cmd);
	g_triggerbot.run(cmd);
	g_knifebot.run(cmd);
	g_aimbot.run(cmd);
	//g_antiaim.run(cmd);
	g_speed_hack.run(cmd, m_speed);

	return false;
}

/*m_writeusercmd_to_deltabuffer_fn hooks::WriteUsercmdDeltaToBuffer_o = nullptr;

// Hooked WriteUsercmdDeltaToBuffer
bool __fastcall hooks::WriteUsercmdDeltaToBuffer_h(void* thisptr, void* buf, int from, int to, bool isnewcommand)
{
	// Invoke the original serialization
	bool ok = WriteUsercmdDeltaToBuffer_o(thisptr, buf, from, to, isnewcommand);

	// Let the tickbase manager inject extra commands if needed
	return g_hooks.tickbase.on_write_usercmd_delta(thisptr, buf, from, to, isnewcommand) ? true : ok;
}

*/



static DWORD WINAPI get_tick_count_h()
{
	static DWORD last_real_tick = 0;
	static DWORD last_fake_tick = 0;

	DWORD current_tick = get_tick_count_original();

	DWORD result;
	if (last_real_tick == 0) {
		last_real_tick = last_fake_tick = current_tick;
		result = current_tick;
	}
	else {
		result = last_fake_tick + static_cast<DWORD>(m_speed * (current_tick - last_real_tick));
		last_fake_tick = result;
		last_real_tick = current_tick;
	}

	return result;
}

static ULONGLONG WINAPI get_tick_count64_h()
{
	static DWORD last_real_tick = 0;
	static DWORD last_fake_tick = 0;

	DWORD current_tick = get_tick_count64_original();

	DWORD result;
	if (last_real_tick == 0) {
		last_real_tick = last_fake_tick = current_tick;
		result = current_tick;
	}
	else {
		result = last_fake_tick + static_cast<ULONGLONG>(m_speed * (current_tick - last_real_tick));
		last_fake_tick = result;
		last_real_tick = current_tick;
	}

	return result;
}

static DWORD WINAPI time_get_time_h()
{
	static DWORD last_real_tick = 0;
	static DWORD last_fake_tick = 0;

	DWORD current_tick = time_get_time_original();

	DWORD result;
	if (last_real_tick == 0) {
		last_real_tick = last_fake_tick = current_tick;
		result = current_tick;
	}
	else {
		result = last_fake_tick + static_cast<ULONGLONG>(m_speed * (current_tick - last_real_tick));
		last_fake_tick = result;
		last_real_tick = current_tick;
	}

	return result;
}

static BOOL WINAPI query_performance_counter_h(LARGE_INTEGER* lpPerformanceCount)
{
	static LARGE_INTEGER last_fake_tick = { 0 };
	static LARGE_INTEGER last_real_tick = { 0 };

	if (!query_performance_counter_original(lpPerformanceCount))
		return FALSE;

	LARGE_INTEGER current_tick = *lpPerformanceCount;

	if (last_real_tick.QuadPart == 0) {
		last_fake_tick = last_real_tick = current_tick;
	}
	else {
		lpPerformanceCount->QuadPart = last_fake_tick.QuadPart + static_cast<LONGLONG>(m_speed * (current_tick.QuadPart - last_real_tick.QuadPart));
		last_fake_tick = *lpPerformanceCount;
		last_real_tick = current_tick;
	}

	return TRUE;
}

void hooks::initialize()
{
	if (MH_Initialize() != MH_OK)
		return;

	auto get_virtual = [](void* _class, unsigned int index) {
		return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
	};

	MH_CreateHook(REINTERPRET_P(get_virtual(g_l4d2.m_client_mode, 27)), create_move_h, REINTERPRET_PP(&create_move_original));
	MH_CreateHook(REINTERPRET_P(get_virtual(g_l4d2.m_device, 17)), present_h, REINTERPRET_PP(&present_original));
	MH_CreateHook(REINTERPRET_P(get_virtual(g_l4d2.m_device, 16)), reset_h, REINTERPRET_PP(&reset_original));
	MH_CreateHook(GetTickCount, get_tick_count_h, REINTERPRET_PP(&get_tick_count_original));
	MH_CreateHook(GetTickCount64, get_tick_count64_h, REINTERPRET_PP(&get_tick_count64_original));
	MH_CreateHook(timeGetTime, time_get_time_h, REINTERPRET_PP(&time_get_time_original));
	MH_CreateHook(QueryPerformanceCounter, query_performance_counter_h, REINTERPRET_PP(&query_performance_counter_original));

	/*auto addr = PatternScanner::Scan(g_modules.engine, patterns::WriteUsercmdDeltaToBuffer);
	if (addr)
	{
		MH_CreateHook(addr,
			&hooks::WriteUsercmdDeltaToBuffer_h,
			reinterpret_cast<void**>(&WriteUsercmdDeltaToBuffer_o));
	}*/
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return;
}

void hooks::undo()
{
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}
