#pragma once
#include "c_keyvalues.h"
#include "../../pattern_list/pattern_list.h"

bool global_keyvalues::LoadFromBuffer(keyvalues* pKval, const char* const szName, const char* const szBuff, void* pSys, const char* const szPath)
{
	m_pattern m_load_from_buffer_fn;

	if (!m_load_from_buffer_fn)
		return false;

	return reinterpret_cast<int(__thiscall*)(keyvalues*, char const*, const char*, void*, const char*)>(m_load_from_buffer_fn)(pKval, szName, szBuff, pSys, szPath);
}

keyvalues* global_keyvalues::Init(keyvalues* pKval, const char* const szName)
{
	global_keyvalues::Init m_key_vals_init_fn = reinterpret_cast<fnKeyValuesInit>(pattern_list::m_key_vals_init_fn);
	if (!m_key_vals_init_fn)
		return nullptr;

	return reinterpret_cast<keyvalues * (__thiscall*)(keyvalues*, const char*)>(m_key_vals_init_fn)(pKval, szName);
}