// This <MIM hack is completely untested and possibly doesn't even work. :quate:
// If anybody wants to try it out, let me know how it goes

#include "MIM.h"
#include "doukutsu/flags.h"
#include "doukutsu/player.h"
#include "doukutsu/npc.h"

namespace custom_tsc_cmds
{
using patcher::dword;
using patcher::byte;
namespace MIM
{

dword* mimFlag = reinterpret_cast<dword*>(&csvanilla::gFlagNPC[996]);

void shiftNPCrect(csvanilla::NPCHAR* npc);
void equipItem(int flag, csvanilla::BOOL b);
void patchMIM()
{
	// Just gonna copy the XML here
	const byte patchPutMyChar[] = {0xA1, 0x84, 0xE1, 0x49, 0x00, 0xC1, 0xE0, 0x05, 0x01, 0x45,
		0xF4, 0x01, 0x45, 0xFC, 0xEB, 0x0C};
	patcher::patchBytes(0x4154B8, patchPutMyChar, sizeof patchPutMyChar);

	// Patch NPC 150, 111, and 112
	constexpr dword addresses[] = {0x445D57, 0x43D003, 0x43D291};
	for (dword address : addresses)
	{
		// Just by coincidence, the same exact patch works for all 3 of these NPCs :)
		const byte patch1[] = {0xFF, 0x75, 0x08}; // PUSH DWORD PTR [EBP+08]
		patcher::patchBytes(address, patch1, sizeof patch1);
		address += sizeof patch1;
		patcher::writeCall(address, shiftNPCrect); // Call our function to shift the NPC rects
		address += 5; // A CALL is 5 bytes
		const byte patch2[] = {
			0x59,      // POP ECX
			0xEB, 0x1E // JMP (past the residual Mimiga Mask equip code)
		};
		patcher::patchBytes(address, patch2, sizeof patch2);
	}

	// Patch equip flag
	patcher::replaceFunction(csvanilla::EquipItem, equipItem);
}

void shiftNPCrect(csvanilla::NPCHAR* npc)
{
	// Get flag used by the unobtrusive <MIM hack
	npc->rect.top += *mimFlag * 32;
	npc->rect.bottom += *mimFlag * 32;
}
void equipItem(int flag, csvanilla::BOOL b)
{
	// Allows <EQ[+/-]0064 to still equip/unequip the Mimiga Mask if not using <MIM
	using csvanilla::gMC;
	if (b)
	{
		gMC.equip |= flag;
		// Make <EQ+0064 set <MIM0001 if MIM is currently 0 (leave it alone otherwise)
		if (flag & 0x40 && *mimFlag == 0)
			*mimFlag = 1;
	}
	else
	{
		gMC.equip &= ~flag;
		// If <EQ-0064, set <MIM0000 if MIM was 1
		if (flag & 0x40 && *mimFlag == 1)
			*mimFlag = 0;
	}
}

} // namespace MIM
} // namespace custom_tsc_cmds
