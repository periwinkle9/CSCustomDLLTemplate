#pragma once

#include <filesystem>

namespace wav_sfx
{
bool loadSfx(const std::filesystem::path& path, int sfxNum);
void applyPatch();
}
