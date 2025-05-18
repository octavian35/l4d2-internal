#include "pattern_list.h"

pattern_list g_pl;

void pattern_list::initialize()
{
	m_device_fn = pattern_scan(X_("shaderapidx9.dll"), X_("A1 ? ? ? ? 8B 08 6A 03")) + 1;
	m_client_mode_fn = pattern_scan(X_("client.dll"), X_("89 04 B5 ? ? ? ? E8")) + 3;
	m_client_state_fn = pattern_scan(X_("engine.dll"), X_(" E8 ? ? ? ? E8 ? ? ? ? 80 3D")) + 1;
	m_get_weapon_data_fn = pattern_scan(X_("client.dll"), X_("0F B7 ? ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3"));
	//m_load_from_buffer_fn = pattern_scan(X_("client.dll"), X_("55 8B EC 83 EC 34 57 8B 7D 0C 89 4D FC 85 FF 75 09"));
	//m_key_vals_init_fn = pattern_scan(X_("client.dll"), X_("55 8B EC 33 C0 56 8B F1 C7 06 ? ? ? ? 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C 66 89 46 10 89 46 20 66 89 46 12 FF 15 ? ? ? ? 8B 4D 08 8B 10 8B 52 0C 6A 01 51 8B C8 FF D2 89 06 8B C6 5E 5D C2 04 00"));
	//m_writeusecmd_to_deltabuffter_fn = pattern_scan(X_("engine.dll"), X_("89 45 ? 56 8B 75 ? 83 BE ? ? ? ? ? 74 ? 8B 4D ? 8D 45 ? 50 51 8D 55"));
}

m_pattern pattern_list::pattern_scan(const char* module_name, const char* signature)
{
	const auto module_handle = GetModuleHandleA(module_name);

	if (!module_handle)
		return nullptr;

	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &scan_bytes[i];
	}
}