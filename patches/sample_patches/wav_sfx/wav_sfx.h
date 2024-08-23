/* Allows sounds in WAV format to be loaded from the data/sfx folder.
   To replace sound 5 (for example), place a file called 5.wav in that folder.
   (It can also be named 0005.wav or any numeric value that can be interpreted as being the number 5.)
   The WAV file must be in PCM format.

   Note that this hack can only REPLACE vanilla sound effects; it cannot add new ones.
   Thus, you are limited to using sound effect numbers between 0-159 (inclusive).
*/

#pragma once

#include <filesystem>

namespace wav_sfx
{
bool loadSfx(const std::filesystem::path& path, int sfxNum);
void applyPatch();
}
