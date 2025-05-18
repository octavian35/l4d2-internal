#pragma once
// redo the hook shit for um you know delta buffer
// #include "../tickbase_manager/tickbase_manager.h"
// #include "../sdk/movement/c_user_cmd.h"
#include "../../libraries/minhook/minhook.h"

#define REINTERPRET_P reinterpret_cast<void*>
#define REINTERPRET_PP reinterpret_cast<void**>

struct hooks {
public:
	void initialize();
	void undo();
	//static m_writeusercmd_to_deltabuffer_fn WriteUsercmdDeltaToBuffer_o;
/*private:
	static bool __fastcall WriteUsercmdDeltaToBuffer_h(void* thisptr, void* edx,
		void* buf, int from, int to, bool isnewcommand);
	// other hooks
	TickbaseManager tickbase; */
};

extern hooks g_hooks;