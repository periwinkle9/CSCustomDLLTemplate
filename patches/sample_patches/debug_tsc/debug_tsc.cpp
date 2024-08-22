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
 - Commands targeting an NPC event number failing to find any such NPC
   (this is often due to the scripter specifying the NPC type instead of the event number)
*/

#include "debug_tsc.h"
#include "patch_utils.h"

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "doukutsu/keybinds.h"
#include "doukutsu/tsc.h"
#include "doukutsu/window.h"

namespace debug_tsc
{
void reportProblem(const std::string& problemText)
{
	MessageBoxA(csvanilla::ghWnd, problemText.c_str(), "TSC issue detected", MB_OK | MB_ICONWARNING);
	csvanilla::gKey = csvanilla::gKeyTrg = 0; // Reset held keys to prevent stuck keys
}

// Replacement for TSC loading functions (to perform static analysis)
// Defined in tsc_load.cpp
csvanilla::BOOL LoadTextScript2(const char* name);
csvanilla::BOOL LoadTextScript_Stage(const char* name);

// Runtime checks during TSC execution
// Defined in tsc_exec_checks.cpp
csvanilla::BOOL handleEventNotFound(int eventNum, csvanilla::BOOL result);
int checkUnexpectedText();

// Parameter checks for TSC commands (WIP-ish)
// Defined in npc_cmds.cpp
void SetFrameTargetNpChar(int event, int wait);
void ChangeNpCharByEvent(int eventNum, int npcType, int direction);
void ChangeCheckableNpCharByEvent(int eventNum, int npcType, int direction);
void SetNpCharActionNo(int eventNum, int actNo, int direction);
void MoveNpChar(int eventNum, int x, int y, int direction);

// Patches StartTextScript() and JumpTextScript() to call our function at the end to handle the return value
// (The vanilla game normally ignores the return value from these functions)
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

// Patches TextScriptProc() at the point where it would print text, to call our function to check for an active message box
// and also whether or not we reached the end of an event without seeing an <END
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

#define patchCall(addr, func) if (patcher::verifyBytes(addr, origBytes_ ## addr, sizeof origBytes_ ## addr)) patcher::writeCall(addr, func)
// Patches TextScriptProc() when parsing certain NPC-related commands, to call our function instead of the vanilla one
bool patchNPCcmds()
{
	using patcher::byte;
	const byte origBytes_0x4241C0[] = {0xE8, 0x8B, 0xB0, 0xFE, 0xFF}; // SetFrameTargetNpChar
	const byte origBytes_0x424705[] = {0xE8, 0xA6, 0xB3, 0x04, 0x00}; // ChangeNpCharByEvent
	const byte origBytes_0x4247AB[] = {0xE8, 0xE0, 0xB7, 0x04, 0x00}; // SetNpCharActionNo
	const byte origBytes_0x424853[] = {0xE8, 0xB8, 0xB4, 0x04, 0x00}; // ChangeCheckableNpCharByEvent
	const byte origBytes_0x424A06[] = {0xE8, 0x55, 0xB6, 0x04, 0x00}; // MoveNpChar
	patchCall(0x4241C0, SetFrameTargetNpChar);
	patchCall(0x424705, ChangeNpCharByEvent);
	patchCall(0x4247AB, SetNpCharActionNo);
	patchCall(0x424853, ChangeCheckableNpCharByEvent);
	patchCall(0x424A06, MoveNpChar);

	// I don't particularly care if any of these patches failed (due to another ASM hack modifying that space)
	return true;
}

bool applyPatch()
{
	patcher::replaceFunction(csvanilla::LoadTextScript2, LoadTextScript2);
	patcher::replaceFunction(csvanilla::LoadTextScript_Stage, LoadTextScript_Stage);
	return patchStartJumpTextScript() && patchTSCParserHook() && patchNPCcmds();
}
} // namespace debug_tsc
