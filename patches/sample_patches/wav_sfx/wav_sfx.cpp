#include "wav_sfx.h"

#include "patch_utils.h"
#include "doukutsu/audio.h"
#include "doukutsu/misc.h"
#include "doukutsu/window.h"

#include <fstream>
#include <string>
#include <sstream>
#include <string_view>
#include <filesystem>
#include <stdexcept>

namespace
{
std::string loadFile(const std::filesystem::path& filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);
	if (!ifs)
		return std::string{};
	std::ostringstream oss;
	oss << ifs.rdbuf();
	return oss.str();
}

WORD readWord(std::string_view str)
{
	return static_cast<unsigned char>(str[0]) | (static_cast<unsigned char>(str[1]) << 8);
}
DWORD readDword(std::string_view str)
{
	DWORD val = 0;
	for (int i = 0; i < 4; ++i)
		val |= (static_cast<unsigned char>(str[i]) << (i * 8));
	return val;
}
} // end anonymous namespace

namespace wav_sfx
{

constexpr int MaxSfx = 160; // size of lpSECONDARYBUFFER array

struct WavInfo
{
	bool valid;
	WAVEFORMATEX fmt;
	unsigned int dataIdx;
	unsigned int dataSize;
};

WavInfo parseWavHeader(std::string_view wavData)
{
	WavInfo info;
	info.valid = false;
	// Check RIFF header
	if (!wavData.starts_with("RIFF") || !wavData.substr(8).starts_with("WAVE"))
		return info;
	unsigned idx = 12; // Start of first subchunk
	bool foundFmt = false, foundData = false;
	while (idx + 8 < wavData.size() && (!foundFmt || !foundData))
	{
		std::string_view chunkName = wavData.substr(idx, 4);
		unsigned chunkSize = readDword(wavData.substr(idx + 4));

		// Get name of this subchunk
		if (chunkName == "fmt ")
		{
			// TODO look into possibly supporting non-PCM WAV files in the future
			// (What formats does DirectSound even support?)
			WORD formatTag = readWord(wavData.substr(idx + 8));

			if (formatTag != WAVE_FORMAT_PCM || chunkSize < 16)
				break;
			// If chunkSize > 16, let's just hope that the extra bytes are safe to ignore
			foundFmt = true;
			info.fmt.wFormatTag = formatTag;
			info.fmt.nChannels = readWord(wavData.substr(idx + 10));
			info.fmt.nSamplesPerSec = readDword(wavData.substr(idx + 12));
			info.fmt.nAvgBytesPerSec = readDword(wavData.substr(idx + 16));
			info.fmt.nBlockAlign = readWord(wavData.substr(idx + 20));
			info.fmt.wBitsPerSample = readWord(wavData.substr(idx + 22));
			info.fmt.cbSize = 0;
		}
		else if (chunkName == "data")
		{
			foundData = true;
			info.dataIdx = idx + 8;
			info.dataSize = chunkSize;
		}

		idx += (chunkSize + 8);
	}

	if (foundFmt && foundData)
		info.valid = true;

	return info;
}

bool loadSfx(const std::filesystem::path& path, int sfxNum)
{
	// Some safety checks just to be sure
	if (sfxNum < 0 || sfxNum >= MaxSfx || csvanilla::lpDS == nullptr)
		return false;

	std::string file = loadFile(path);
	if (file.empty())
		return false;

	using csvanilla::lpSECONDARYBUFFER;
	auto freeSoundBuffer = [sfxNum]()
	{
		if (lpSECONDARYBUFFER[sfxNum] != nullptr)
		{
			lpSECONDARYBUFFER[sfxNum]->Release();
			lpSECONDARYBUFFER[sfxNum] = nullptr;
		}
	};

	freeSoundBuffer();

	WavInfo wavInfo = parseWavHeader(file);
	if (!wavInfo.valid)
	{
#ifdef _DEBUG
		std::string warningMsg = wavPath.string() + ": unrecognized or invalid WAV format";
		OutputDebugStringA(warningMsg.c_str());
#endif
		return false;
	}

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof dsbd);
	dsbd.dwSize = sizeof dsbd;
	dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = wavInfo.dataSize;
	dsbd.lpwfxFormat = &wavInfo.fmt;

	LPVOID lpbuf1, lpbuf2;
	DWORD dwbuf1, dwbuf2;
	if (csvanilla::lpDS->CreateSoundBuffer(&dsbd, &lpSECONDARYBUFFER[sfxNum], nullptr) != DS_OK ||
		lpSECONDARYBUFFER[sfxNum]->Lock(0, wavInfo.dataSize, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0) != DS_OK)
	{
		freeSoundBuffer();
		return false;
	}
	CopyMemory(lpbuf1, file.data() + wavInfo.dataIdx, dwbuf1);
	if (dwbuf2 != 0)
		CopyMemory(lpbuf2, file.data() + wavInfo.dataIdx + dwbuf1, dwbuf2);
	lpSECONDARYBUFFER[sfxNum]->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2);

	return true;
}

void loadSfxFolder()
{
	namespace fs = std::filesystem;

	fs::path sfxDir = fs::path{csvanilla::gDataPath} / "sfx";
	if (!fs::is_directory(sfxDir))
		return;

	bool loadedSfx[MaxSfx] = {};
	for (const fs::directory_entry& dirEntry : fs::directory_iterator{sfxDir})
	{
		fs::path sfxPath = dirEntry.path();
		if (dirEntry.is_regular_file() && sfxPath.extension() == ".wav")
		{
			try
			{
				int sfxNum = std::stoi(sfxPath.stem(), nullptr, 10);
				// If out of range or already loaded a sfx for this number, skip this file
				if (sfxNum < 0 || sfxNum >= MaxSfx || loadedSfx[sfxNum])
					continue;

				loadedSfx[sfxNum] = loadSfx(sfxPath, sfxNum);
			}
			catch (const std::invalid_argument&)
			{
				continue;
			}
			catch (const std::out_of_range&)
			{
				continue;
			}
		}
	}
}

void replacesPlaySoundObjectCall(int soundNum, int playMode)
{
	loadSfxFolder();
	return csvanilla::PlaySoundObject(soundNum, playMode);
}

void applyPatch()
{
	// Replace call to PlaySoundObject(7, -1); in Game() after LoadGenericData()
	// I *would* just call loadSfx() in applyPostInitPatches(), but that function runs
	// after the aforementioned PlaySoundObject call, so we had better load all of the
	// sound effects before then
	patcher::writeCall(0x40F62B, replacesPlaySoundObjectCall);
}

} // end namespace wav_sfx
