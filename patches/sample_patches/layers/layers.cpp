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
int screenTileWidth = 21;
int screenTileHeight = 16;

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

void ReadLayerData(unsigned short* dest, int size, csvanilla::FILE* fp)
{
	csvanilla::fread(dest, 2, size, fp);
}

// Replaces the fread(gMap.data, 1, gMap.width * gMap.length, fp); call at the end of LoadMapData2()
void LoadMapData2_hook(void*, unsigned pxmLayersCheck, unsigned mapSize, csvanilla::FILE* fp)
{
	// Check if this is a layers PXM
	if ((pxmLayersCheck & 0xFF000000) == 0x21000000)
	{
		ReadLayerData(gLayers.farBackData, mapSize, fp);
		ReadLayerData(gLayers.backData, mapSize, fp);
		ReadLayerData(gLayers.data, mapSize, fp);
		ReadLayerData(gLayers.frontData, mapSize, fp);
	}
	else
	{
		// Otherwise, load this like a normal map
		csvanilla::memset(gLayers.farBackData, 0, mapSize * 2);
		csvanilla::memset(gLayers.backData, 0, mapSize * 2);
		csvanilla::memset(gLayers.frontData, 0, mapSize * 2);

		unsigned char* tmp = (unsigned char*)csvanilla::malloc(mapSize);
		csvanilla::fread(tmp, 1, mapSize, fp);
		for (unsigned i = 0; i < mapSize; ++i)
		{
			gLayers.data[i] = tmp[i];
		}
		csvanilla::free(tmp);
	}
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
void PutStage_Layer(unsigned short* data, int fx, int fy, Func1 skipCond, Func2 drawExtra)
{
	using namespace csvanilla;

	// Get range to draw
	const int put_x = ((fx / 0x200) + 8) / 16;
	const int put_y = ((fy / 0x200) + 8) / 16;
	
	for (int j = put_y; j < put_y + screenTileHeight; ++j)
	{
		for (int i = put_x; i < put_x + screenTileWidth; ++i)
		{
			// Get attribute
			int offset = (j * gMap.width) + i;
			int atrb = GetAttribute(i, j);

			if (skipCond(atrb))
				continue;

			// Draw tile
			::RECT rect;
			rect.left = (data[offset] % 16) * 16;
			rect.top = (data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			int x = ((i * 16) - 8) - (fx / 0x200), y = ((j * 16) - 8) - (fy / 0x200);
			PutBitmap3(&grcGame, x, y, &rect, 2);

			// For PutStage_Front
			if (atrb == 0x43)
				drawExtra(x, y);
		}
	}
}

// Separating this out is assured to have virtually no performance impact, but
// people have been having lag issues with this hack, so I may as well try anyways
void PutStage_Layer(unsigned short* data, int fx, int fy)
{
	using namespace csvanilla;

	// Get range to draw
	const int put_x = ((fx / 0x200) + 8) / 16;
	const int put_y = ((fy / 0x200) + 8) / 16;

	for (int j = put_y; j < put_y + screenTileHeight; ++j)
	{
		for (int i = put_x; i < put_x + screenTileWidth; ++i)
		{
			int offset = (j * gMap.width) + i;

			// Draw tile
			::RECT rect;
			rect.left = (data[offset] % 16) * 16;
			rect.top = (data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;
			
			PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, 2);
		}
	}
}

void PutStage_Back(int fx, int fy)
{
	// Draw back layers
	PutStage_Layer(gLayers.farBackData, fx, fy);
	PutStage_Layer(gLayers.backData, fx, fy);

	// This is vanilla behavior
	PutStage_Layer(gLayers.data, fx, fy, [](int atrb) { return atrb >= 0x20; }, [](int, int) {});
}

void PutStage_Front(int fx, int fy)
{
	// This is vanilla behavior
	PutStage_Layer(gLayers.data, fx, fy, [](int atrb) { return atrb < 0x40 || atrb >= 0x80; },
		[](int x, int y)
		{
			const ::RECT rcSnack = {256, 48, 272, 64};
			csvanilla::PutBitmap3(&csvanilla::grcGame, x, y, &rcSnack, 20);
		});

	// Draw the far-front layer
	PutStage_Layer(gLayers.frontData, fx, fy);
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
	// Pass PXM header dummy byte into LoadMapData2_hook to check for layers PXM
	const byte patchArgs[] = {0xFF, 0x75, 0xF4, 0xFF, 0x35, 0x80, 0xE4, 0x49, 0x00};
	patcher::patchBytes(0x41386F, patchArgs, sizeof patchArgs);
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

void fixGraphicsEnhancementCompatibility()
{
	// In case of graphics_enhancement with widescreen, reapply the GetAttribute() patch
	patcher::replaceFunction(csvanilla::GetAttribute, GetAttribute);
	// Get screen dimensions (in case of widescreen)
	ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x413AF9), &screenTileWidth, 4, NULL);
	// The modloader hack doesn't write to this one?
	//ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x413B00), &screenTileHeight, 4, NULL);
}

}
