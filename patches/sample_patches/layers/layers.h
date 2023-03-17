#pragma once

namespace layers_mode
{

// Replacing gMap entirely is too much hassle, so let's just make a new struct with the added data
struct LAYERSDATA
{
	unsigned short* data;
	unsigned short* farBackData;
	unsigned short* backData;
	unsigned short* frontData;
	unsigned char atrb[0x10000]; // Expanded gMap.atrb to support 16-bit PXMs
};

extern LAYERSDATA gLayers;

void applyLayersPatch();
void applyTSCPatch();

inline void applyPatch()
{
	applyLayersPatch();
	applyTSCPatch();
}

}
