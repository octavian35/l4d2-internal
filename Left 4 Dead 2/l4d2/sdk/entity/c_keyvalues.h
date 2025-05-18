#pragma once

#include "c_base_entity.h"

class global_keyvalues
{
public:
	using KeyValuesInitFn = keyvalues * (__thiscall*)(keyvalues* thisptr, const char* name);
	bool LoadFromBuffer(keyvalues* pKval, const char* const szName, const char* const szBuff, void* pSys = nullptr, const char* const szPath = nullptr);
	keyvalues* Init(keyvalues* pKval, const char* const szName);
};

namespace G { inline global_keyvalues keyvalues; }
