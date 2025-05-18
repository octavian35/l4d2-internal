#pragma once
#include "../"
class global_entity_vars {
public:
	bool IsInfectedAlive(const int nSolidFlags, const int nSequence, int classindex);
};

namespace G { inline global_entity_vars util; }