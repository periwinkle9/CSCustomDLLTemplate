#pragma once

namespace cs_60fps_patch
{
void applyFPSPatch();
void applyTimerPatch();
inline void applyPatches()
{
	applyFPSPatch();
	applyTimerPatch();
}
}
