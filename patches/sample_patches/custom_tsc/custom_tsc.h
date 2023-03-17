#pragma once

#include "MIM.h"

namespace custom_tsc_cmds
{
void patchTSCHook();

// If any of your custom TSC commands need extra ASM patches in order to function,
// add them into this function.
inline void applyPatch()
{
	patchTSCHook();
	MIM::patchMIM();
}

}
