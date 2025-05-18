#pragma once

#include "../../std.h"

typedef uint8_t* m_pattern;

struct pattern_list {
public:
	void initialize();
public:
	m_pattern m_device_fn;
	m_pattern m_client_mode_fn;
	m_pattern m_client_state_fn;
	m_pattern m_get_weapon_data_fn;
	//m_pattern m_writeusecmd_to_deltabuffter_fn;
	//m_pattern m_load_from_buffer_fn;
	//m_pattern m_key_vals_init_fn;
private:
	m_pattern pattern_scan(const char* module_name, const char* signature);
};

extern pattern_list g_pl;