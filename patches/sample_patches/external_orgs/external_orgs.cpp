// External ORG hack - see external_orgs.h for more information.

/*
Technical details:
This patch changes the LoadOrganya(const char* name) and OrgData::InitMusicData(const char* path) functions
so that instead of the track name, the first argument should be a pointer to the org data itself.
The task of actually loading the org file into memory is then offloaded to this DLL.
*/
#include "external_orgs.h"
#include "patch_utils.h"
#include "doukutsu/audio.h"
#include "doukutsu/organya.h"
#include "doukutsu/window.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <format>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace external_orgs
{
constexpr int VanillaMusicTableSize = 42;

class OrgBuffer
{
	char* data;
	bool isFromFile;

	void reset();
public:
	OrgBuffer() : data{nullptr}, isFromFile{false} {}
	~OrgBuffer() { reset(); }
	// Too lazy to properly implement rule of 5 (not that we need these anyways)
	OrgBuffer(const OrgBuffer&) = delete;
	OrgBuffer(OrgBuffer&&) = delete;
	OrgBuffer& operator=(const OrgBuffer&) = delete;
	OrgBuffer& operator=(OrgBuffer&&) = delete;

	void loadFromFile(const std::filesystem::path& filePath);
	void loadFromResource(const char* name);

	const char* get() { return data; }
	explicit operator bool() const { return data != nullptr; }
};

void OrgBuffer::reset()
{
	if (data != nullptr && isFromFile)
	{
		delete data;
		data = nullptr;
	}
}

void OrgBuffer::loadFromFile(const std::filesystem::path& filePath)
{
	reset();
	std::ifstream ifs{filePath, std::ios::binary};
	if (!ifs)
	{
		data = nullptr;
		return;
	}
	auto fileSize = std::filesystem::file_size(filePath);
	data = new char[fileSize];
	isFromFile = true;
	ifs.read(data, fileSize);
}

void OrgBuffer::loadFromResource(const char* name)
{
	reset();
	HRSRC hrscr = FindResourceA(NULL, name, "ORG");
	if (!hrscr)
		return;
	HGLOBAL resHandle = LoadResource(NULL, hrscr);
	if (!resHandle)
		return;
	data = static_cast<char*>(LockResource(resHandle));
	isFromFile = false;
}

// The vanilla function takes the track name as a string parameter, but here we'll change that to an int
BOOL LoadOrganyaWrapper(int musicNum)
{
	namespace fs = std::filesystem;
	std::string fileName = std::format("{:04}.org", musicNum);
	fs::path orgPath = fs::path{csvanilla::gDataPath} / "Org" / fileName;

	OrgBuffer orgData;
	if (fs::is_regular_file(orgPath))
		orgData.loadFromFile(orgPath);
	else if (musicNum >= 0 && musicNum < VanillaMusicTableSize)
		orgData.loadFromResource(csvanilla::gMusicTable[musicNum]);
	else // Load track 0 by default
		orgData.loadFromResource(csvanilla::gMusicTable[0]);

	if (orgData)
		return csvanilla::LoadOrganya(orgData.get());
	else
		return FALSE;
}


// Patching functions

bool verifyIntegrity()
{
	using patcher::byte;
	const byte origInitMusicData[] = {0x68, 0xD4, 0xC6, 0x48, 0x00}; // push 48C6D4
	const byte origChangeMusic[] = {
		0x8B, 0x55, 0x08, // mov edx, dword ptr [ebp+8]
		0x8B, 0x04, 0x95, 0xE8, 0x81, 0x49, 0x00, // mov eax, dword ptr [edx*4+4981E8]
		0x50, // push eax
		0xE8, 0xCF, 0xB7, 0xFF, 0xFF // call 41C6F0
	};
	const byte origReCallMusic[] = {
		0xA1, 0xFC, 0x57, 0x4A, 0x00, // mov eax, dword ptr [4A57FC] (gOldNo)
		0x8B, 0x0C, 0x85, 0xE8, 0x81, 0x49, 0x00, // mov ecx, dword ptr [eax*4+4981E8]
		0x51, // push ecx
		0xE8, 0x86, 0xB7, 0xFF, 0xFF // call 41C6F0
	};

	return patcher::verifyBytes(0x41BAEC, origInitMusicData, sizeof origInitMusicData) &&
		patcher::verifyBytes(0x420F11, origChangeMusic, sizeof origChangeMusic) &&
		patcher::verifyBytes(0x420F58, origReCallMusic, sizeof origReCallMusic);
}

void patchInitMusicData()
{
	// Patches OrgData::InitMusicData() to jump past the resource-loading part,
	// and just treat the function argument as the org data
	using patcher::byte;
	const byte patchBytes[] = {
		0x8B, 0x45, 0x08, // mov eax, dword ptr [ebp+8]
		0xEB, 0x2F        // jmp 41BB20 (past the LockResource() call)
	};
	patcher::patchBytes(0x41BAEC, patchBytes, sizeof patchBytes);
}

void patchLoadOrganyaCalls()
{
	using patcher::byte;

	// Patch LoadOrganya() argument and call in ChangeMusic() with our function and the appropriate argument
	patcher::writeNOPs(0x420F14, 7);
	byte pushRegister = 0x52; // push edx (track number)
	patcher::patchBytes(0x420F1B, &pushRegister, 1);
	patcher::writeCall(0x420F1C, LoadOrganyaWrapper);

	// Do the same for the LoadOrganya() call in ReCallMusic()
	patcher::writeNOPs(0x420F5D, 7);
	pushRegister = 0x50; // push eax
	patcher::patchBytes(0x420F64, &pushRegister, 1);
	patcher::writeCall(0x420F65, LoadOrganyaWrapper);
}

bool applyPatch()
{
	if (!verifyIntegrity())
		return false;

	patchInitMusicData();
	patchLoadOrganyaCalls();
	return true;
}

} // end namespace external_orgs
