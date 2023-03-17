// Layers mode implementation
// idk how this even works, I'm just copy/pasting code (mostly)

#include "layers.h"
#include "patch_utils.h"
#include "doukutsu/cstdlib.h"
#include "doukutsu/map.h"
#include "doukutsu/draw.h"
#include "doukutsu/npc.h"

namespace layers_mode
{

constexpr int PXM_BUFFER_SIZE = 0x96000; // Double the vanilla size

LAYERSDATA gLayers;

csvanilla::BOOL InitMapData2()
{
	using csvanilla::malloc;
	gLayers.data = (unsigned short*)malloc(PXM_BUFFER_SIZE);
	gLayers.farBackData = (unsigned short*)malloc(PXM_BUFFER_SIZE);
	gLayers.backData = (unsigned short*)malloc(PXM_BUFFER_SIZE);
	gLayers.frontData = (unsigned short*)malloc(PXM_BUFFER_SIZE);
	return 1;
}

void ReadLayerData(unsigned short* dest, int half_size, csvanilla::FILE* fp)
{
	unsigned short tmp;
	for (int i = 0; i < half_size; ++i)
	{
		csvanilla::fread(&tmp, 2, 1, fp);
		dest[i] = tmp;
	}
}

// Replaces the fread(gMap.data, 1, gMap.width * gMap.length, fp); call at the end of LoadMapData2()
void LoadMapData2_hook(void*, unsigned, unsigned mapSize, csvanilla::FILE* fp)
{
	ReadLayerData(gLayers.farBackData, mapSize, fp);
	ReadLayerData(gLayers.backData, mapSize, fp);
	ReadLayerData(gLayers.data, mapSize, fp);
	ReadLayerData(gLayers.frontData, mapSize, fp);
}

void EndMapData()
{
	using csvanilla::free;
	free(gLayers.data);
	free(gLayers.farBackData);
	free(gLayers.backData);
	free(gLayers.frontData);
}

// Since we're making gMap.data an unsigned short* instead of unsigned char*, that changes all of the
// offsets for the array indexes, so I guess I have to replace every function that uses gMap.data... :/
unsigned char GetAttribute(int x, int y)
{
	using csvanilla::gMap;
	if (x < 0 || y < 0 || x >= gMap.width || y >= gMap.length)
		return 0;
	return gLayers.atrb[gLayers.data[x + y * gMap.width]];
}
void DeleteMapParts(int x, int y)
{
	gLayers.data[x + y * csvanilla::gMap.width] = 0;
}
void ShiftMapParts(int x, int y)
{
	--gLayers.data[x + y * csvanilla::gMap.width];
}
csvanilla::BOOL ChangeMapParts(int x, int y, unsigned short no)
{
	unsigned short& tile = gLayers.data[x + y * csvanilla::gMap.width];
	if (tile == no)
		return 0;
	tile = no;
	for (int i = 0; i < 3; ++i)
		csvanilla::SetNpChar(4, x * 0x2000, y * 0x2000, 0, 0, 0, nullptr, 0);
	return 1;
}

template <typename Func1, typename Func2>
void PutStage_Layer(unsigned short* data, int fx, int fy, Func1 skipCond, Func2 extra)
{
	using namespace csvanilla;

	int i, j;
	::RECT rect;
	int offset;

	// Get range to draw
	int num_x = 21;
	int num_y = 16;
	int put_x = ((fx / 0x200) + 8) / 16;
	int put_y = ((fy / 0x200) + 8) / 16;

	int atrb;

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			offset = (j * gMap.width) + i;
			atrb = GetAttribute(i, j);

			if (skipCond(atrb))
				continue;

			// Draw tile
			rect.left = (data[offset] % 16) * 16;
			rect.top = (data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, 2);

			// For PutStage_Front
			extra(atrb, i, j, fx, fy);
		}
	}
}

static bool noSkip(int)
{
	return false;
}
static void nop(int, int, int, int, int)
{}

void PutStage_Back(int fx, int fy)
{
	// Draw back layers
	PutStage_Layer(gLayers.farBackData, fx, fy, noSkip, nop);
	PutStage_Layer(gLayers.backData, fx, fy, noSkip, nop);

	// This is vanilla behavior
	PutStage_Layer(gLayers.data, fx, fy, [](int atrb) { return atrb >= 0x20; }, nop);
}

void PutStage_Front(int fx, int fy)
{
	// This is vanilla behavior
	PutStage_Layer(gLayers.data, fx, fy, [](int atrb) { return atrb < 0x40 || atrb >= 0x80; },
		[](int atrb, int i, int j, int fx, int fy)
		{
			const ::RECT rcSnack = {256, 48, 272, 64};
			if (atrb == 0x43)
				csvanilla::PutBitmap3(&csvanilla::grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rcSnack, 20);
		});

	// Draw the far-front layer
	PutStage_Layer(gLayers.frontData, fx, fy, noSkip, nop);
}

csvanilla::BOOL RecreateTilesetSurface(const char* name, int surf_no)
{
	csvanilla::ReleasePartsImage();
	return csvanilla::MakeSurface_File(name, surf_no);
}


void applyLayersPatch()
{
	using patcher::byte;
	patcher::replaceFunction(csvanilla::InitMapData2, InitMapData2);
	// if (gMap.data == NULL) --> if (gLayers.data == NULL) in LoadMapData2
	const unsigned short* const* const dataPtr = &gLayers.data;
	patcher::patchBytes(0x413842, reinterpret_cast<const byte* const>(&dataPtr), 4);
	// Load layers
	patcher::writeCall(0x413878, LoadMapData2_hook);

	// Increase size of atrb array
	patcher::dword atrbSize = sizeof gLayers.atrb;
	patcher::patchBytes(0x4138F4, reinterpret_cast<byte*>(&atrbSize), 4); // New size to memset
	// Load PXA data into gLayers.atrb instead of gMap.atrb
	void* atrbPtr = &gLayers.atrb;
	patcher::patchBytes(0x4138FB, reinterpret_cast<byte*>(&atrbPtr), 4);

	patcher::replaceFunction(csvanilla::EndMapData, EndMapData);
	patcher::replaceFunction(csvanilla::GetAttribute, GetAttribute);
	patcher::replaceFunction(csvanilla::DeleteMapParts, DeleteMapParts);
	patcher::replaceFunction(csvanilla::ShiftMapParts, ShiftMapParts);
	patcher::replaceFunction(csvanilla::ChangeMapParts, ChangeMapParts);
	// Don't truncate <CMP argument to 1 byte
	byte pushEAX = 0x50;
	patcher::patchBytes(0x424B30, &pushEAX, 1);

	patcher::replaceFunction(csvanilla::PutStage_Back, PutStage_Back);
	patcher::replaceFunction(csvanilla::PutStage_Front, PutStage_Front);

	// Recreate tileset surface when loading a new stage
	patcher::writeCall(0x420C55, RecreateTilesetSurface); // Replaces the call to ReloadBitmap_File()
}

}
