// Implements <CML and <SML for the layers hack.
// Overwrites the space used for <SMP

#include "layers.h"
#include "patch_utils.h"
#include "doukutsu/map.h"
#include "doukutsu/npc.h"
#include "doukutsu/tsc.h"

namespace layers_mode
{

static unsigned short* getTile(int x, int y, int layer)
{
	int off = x + y * csvanilla::gMap.width;
	switch (layer)
	{
	case 0:
		return &gLayers.farBackData[off];
	case 1:
		return &gLayers.backData[off];
	case 2:
		return &gLayers.data[off];
	case 3:
		return &gLayers.frontData[off];
	default:
		return nullptr;
	}
}

void ShiftMapLayer(int x, int y, int layer)
{
	unsigned short* tile = getTile(x, y, layer);
	if (tile != nullptr)
		--(*tile);
}

csvanilla::BOOL ChangeMapLayer(int x, int y, unsigned short no, int layer)
{
	unsigned short* tile = getTile(x, y, layer);
	if (tile == nullptr || *tile == no)
		return 0;
	*tile = no;
	for (int i = 0; i < 3; ++i)
		csvanilla::SetNpChar(4, x * 0x2000, y * 0x2000, 0, 0, 0, nullptr, 0);
	return 1;
}

// Conveniently, <SMP and <CMP are right next to each other in the ASM
bool checkSMPandCMP()
{
	using csvanilla::gTS;
	using csvanilla::GetTextScriptNo;
	// There are obviously more efficient ways of doing this, but it doesn't *really* matter
	char cmd1 = gTS.data[gTS.p_read + 1];
	char cmd2 = gTS.data[gTS.p_read + 2];
	char cmd3 = gTS.data[gTS.p_read + 3];
	if ((cmd1 == 'S' || cmd1 == 'C') && cmd2 == 'M' && (cmd3 == 'P' || cmd3 == 'L'))
	{
		int layer = 2; // Front layer
		int off = 4;
		if (cmd3 == 'L')
		{
			layer = GetTextScriptNo(gTS.p_read + 4);
			off += 5;
		}
		int x = GetTextScriptNo(gTS.p_read + off); off += 5;
		int y = GetTextScriptNo(gTS.p_read + off);
		if (cmd1 == 'C') // <CMP/<CML
		{
			off += 5;
			int no = GetTextScriptNo(gTS.p_read + off);
			ChangeMapLayer(x, y, no, layer);
		}
		else // <SMP/SML
			ShiftMapLayer(x, y, layer);
		
		gTS.p_read += (off + 4);
		return true;
	}
	else
		return false; // Not <SMP or <CMP
}

void applyTSCPatch()
{
	using patcher::byte;

	byte patch[] = {
		//0xE8, 0x00, 0x00, 0x00, 0x00,     // CALL checkCMPandCMP (patched separately)
		0x84, 0xC0,                         // TEST al, al
		0x0F, 0x84, 0x26, 0x01, 0x00, 0x00, // JZ 424B55  ; next command
		0xE9, 0x73, 0x08, 0x00, 0x00        // JMP 4252A7 ; jump out
	};
	patcher::writeCall(0x424A22, checkSMPandCMP);
	patcher::patchBytes(0x424A27, patch, sizeof patch);
}

}
