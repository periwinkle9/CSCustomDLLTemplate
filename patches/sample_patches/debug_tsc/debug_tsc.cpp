/*
This hack doesn't add any additional functionality to the game, but aims to help with debugging TSC errors.
Common TSC errors that this hack can catch:
 - Event numbers not being 4 digits
 - Event numbers out of order
 - '#' characters being used outside of event numbers
 - Attempt to run a nonexistent event
 - Reaching the end of an event without <END
 - Text sounds outside of message boxes (usually more a symptom of other problems)
 - TSC file too big (not a common problem, but can cause strange issues if left undetected)
*/

#include "debug_tsc.h"
#include "patch_utils.h"

#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <cstddef>
#include <cstring>
#include <limits>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "doukutsu/cstdlib.h"
#include "doukutsu/keybinds.h"
#include "doukutsu/misc.h"
#include "doukutsu/player.h"
#include "doukutsu/tsc.h"
#include "doukutsu/window.h"

using std::size_t;

namespace csvanilla
{
const auto msize = reinterpret_cast<size_t(*)(void*)>(0x48AF9F);
}

namespace debug_tsc
{
void reportProblem(const std::string& problemText)
{
	MessageBoxA(csvanilla::ghWnd, problemText.c_str(), "TSC issue detected", MB_OK | MB_ICONWARNING);
	csvanilla::gKey = csvanilla::gKeyTrg = 0; // Reset held keys to prevent stuck keys
}

void checkAndReallocTSCBuffer(size_t scriptSize)
{
	// Possible FIXME: check against original msize rather than current msize
	// (in case of multiple scripts being too large, those other scripts may not be caught immediately)
	if (scriptSize >= csvanilla::msize(csvanilla::gTS.data))
	{
		reportProblem(std::format(
			"Current TSC file is too big ({} bytes).\nPlease use the tsc_malloc hack to increase the max TSC buffer size.",
			scriptSize));
		// Reallocate the buffer as a temporary measure to prevent immediate problems
		csvanilla::realloc(csvanilla::gTS.data, scriptSize + 1); // +1 for the terminating null character
	}
}

// Check a TSC file for common errors:
// - Out of order event #s
// - Event #s not 4 digits
// - Possible usages of '#' character outside of event numbers
// - Script too big
void analyzeTSC(const std::string& script)
{
	checkAndReallocTSCBuffer(script.size());
	int lastEventNumber = std::numeric_limits<int>::min();

	auto getLine = [&script](std::string::size_type pos)
	{
		auto lastNewline = script.find_last_of('\n', pos);
		auto nextNewline = script.find_first_of('\r', pos);
		return script.substr(lastNewline + 1, nextNewline - (lastNewline + 1));
	};
	auto parseEventNumber = [&script](std::string::size_type pos)
	{
		int place = 1000;
		int value = 0;
		for (int i = 0; i < 4 && pos + i < script.size(); ++i, place /= 10)
			value += (script[pos + i] - '0') * place;
		return value;
	};
	for (auto currentPos = script.find('#'); currentPos < script.size(); currentPos = script.find('#', currentPos + 1))
	{
		// Check for # not at the start of the line (which could indicate # being used in a textbox instead of an event number)
		if (currentPos > 0 && script[currentPos - 1] != '\n')
		{
			reportProblem(std::format("Detected event number not at the start of a line (possible erroneous use of # in a textbox?).\n"
				"If this is truly meant to be an event number, it is recommended to put it at the start of the line.\nOffending line: {}", getLine(currentPos)));
		}

		int eventNum = parseEventNumber(currentPos + 1);

		// Check for event number less than 4 digits
		auto endOfLine = script.find('\r', currentPos);
		if (endOfLine == std::string::npos)
			endOfLine = script.size();
		if (endOfLine - currentPos < 5)
		{
			std::string eventNumText = script.substr(currentPos, endOfLine - currentPos);
			reportProblem(std::format("Detected event number ({}) that's not 4 digits.\n"
				"This will be treated as event number {}, which may cause more problems later.", eventNumText, eventNum));
		}
		else if (eventNum < lastEventNumber)
			reportProblem(std::format("Detected out of order event numbers: event {} needs to be before event {}", eventNum, lastEventNumber));
		else if (eventNum == lastEventNumber)
			reportProblem(std::format("Detected duplicate event number: {} (the second event will never be called)", eventNum));
		//else
			lastEventNumber = eventNum;
	}
}

bool loadTSC(const char* fileName, std::string& out)
{
	std::string path = std::string{csvanilla::gDataPath} + '\\' + fileName;
	{
		std::ifstream ifs{path, std::ios::binary};
		if (!ifs)
			return false;
		std::ostringstream oss;
		oss << ifs.rdbuf();
		out = oss.str();
	}
	csvanilla::EncryptionBinaryData2(reinterpret_cast<unsigned char*>(out.data()), out.size());
	return true;
}

csvanilla::BOOL LoadTextScript2(const char* name)
{
	std::string script;
	if (!loadTSC(name, script))
		return 0;
	analyzeTSC(script);

	using csvanilla::gTS;
	std::memcpy(gTS.data, script.data(), script.size());
	gTS.data[script.size()] = '\0';
	gTS.size = static_cast<int>(script.size());
	strcpy_s(gTS.path, name);
	return 1;
}

csvanilla::BOOL LoadTextScript_Stage(const char* name)
{
	std::string head, body;
	if (!loadTSC("Head.tsc", head) || !loadTSC(name, body))
		return 0;
	head += body;
	analyzeTSC(head);

	using csvanilla::gTS;
	std::memcpy(gTS.data, head.data(), head.size());
	gTS.data[head.size()] = '\0';
	gTS.size = static_cast<int>(head.size());
	strcpy_s(gTS.path, name);
	return 1;
}

// Keeping track of the current event number to aid with diagnostic messages
int currentEventNumber;
// Used to prevent repeat warnings about text outside <MSG for each character
bool isInUnprintedText = false;

// To be called at the end of StartTextScript() and JumpTextScript().
// Handles the error condition where the exact event number was not found.
csvanilla::BOOL handleEventNotFound(int eventNum, csvanilla::BOOL result)
{
	isInUnprintedText = false; // Reset this
	if (!result)
	{
		reportProblem(std::format("Failed to find event {}! The event will now be stopped to prevent further issues.", eventNum));
		csvanilla::StopTextScript();
		csvanilla::gMC.cond &= ~1; // <END resets this but not StopTextScript()
	}
	else
		currentEventNumber = eventNum;
	return result;
}

// To be called in the TSC parser when the current script being parsed appears to be text.
// Checks that a message box is currently active, and that we haven't ran off the end of the event.
// (The return value of the function is a value that was overwriten in the original ASM to make room for this function.)
int checkUnexpectedText()
{
	using csvanilla::gTS;
	// Check if we fell off the end of the event
	if (gTS.data[gTS.p_read] == '\0' || gTS.data[gTS.p_read] == '#')
	{
		reportProblem(std::format("Reached the end of the current event ({}) without running an <END.\n"
			"The event will continue running from this point in the script, but this is probably not what you want.", currentEventNumber));
		isInUnprintedText = (gTS.flags & 1) == 0;
	}
	// Check if we're about to print text to an invisible message box
	else if (!(gTS.flags & 1) && gTS.mode != 0) // Last check is in case of handleEventNotFound() stopping the event
	{
		if (!isInUnprintedText)
		{
			isInUnprintedText = true;
			std::string unprintedText;
			for (int pos = gTS.p_read; gTS.data[pos] != '\0' && gTS.data[pos] != '<' && gTS.data[pos] != '\r'; ++pos)
				unprintedText += gTS.data[pos];
			reportProblem(std::format("Interpreting \"{}\" as text, but no message box is active. "
				"(This is usually a symptom of another TSC error.)",
				unprintedText));
		}
	}
	else
		isInUnprintedText = false;

	return gTS.flags; // Replaces the 'movsx eax, byte ptr [4A5ADD]' instruction that was removed to call this function
}

bool patchStartJumpTextScript()
{
	const patcher::byte patch[] = {
		0x50, // push eax ; return value
		0xFF, 0x75, 0x08, // push dword ptr [ebp+8] ; event number
		0xE8, 0x00, 0x00, 0x00, 0x00, // call (our function)
		0xC9, // leave
		0xC3 // retn
	};

	const patcher::dword addrStartTextScript = 0x421AE4;
	const patcher::byte origBytes_StartTextScript[] = {
		0x8B, 0xE5, // mov esp, ebp
		0x5D, // pop ebp
		0xC3, // retn
		0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC // INT3
	};

	const patcher::dword addrJumpTextScript = 0x421C44;
	// Same ASM as above
	const patcher::byte origBytes_JumpTextScript[] = {0x8B, 0xE5, 0x5D, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};

	if (!patcher::verifyBytes(addrStartTextScript, origBytes_StartTextScript, sizeof origBytes_StartTextScript) ||
		!patcher::verifyBytes(addrJumpTextScript, origBytes_JumpTextScript, sizeof origBytes_JumpTextScript))
		return false;

	patcher::patchBytes(addrStartTextScript, patch, sizeof patch);
	patcher::writeCall(addrStartTextScript + 4, handleEventNotFound);
	patcher::patchBytes(addrJumpTextScript, patch, sizeof patch);
	patcher::writeCall(addrJumpTextScript + 4, handleEventNotFound);
	return true;
}

bool patchTSCParserHook()
{
	const patcher::byte origBytes[] = {0x0F, 0xBE, 0x05, 0xDD, 0x5A, 0x4A, 0x00}; // movsx eax, byte ptr [4A5ADD]
	const patcher::dword address = 0x4252FC;
	if (!patcher::verifyBytes(address, origBytes, sizeof origBytes))
		return false;
	patcher::writeNOPs(address, 2);
	patcher::writeCall(address + 2, checkUnexpectedText);
	return true;
}

bool applyPatch()
{
	patcher::replaceFunction(csvanilla::LoadTextScript2, LoadTextScript2);
	patcher::replaceFunction(csvanilla::LoadTextScript_Stage, LoadTextScript_Stage);
	return patchStartJumpTextScript() && patchTSCParserHook();
}
} // namespace debug_tsc
