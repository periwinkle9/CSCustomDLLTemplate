// Custom TSC commands are implemented here

#include "MIM.h"
#include "doukutsu/tsc.h"
#include "doukutsu/player.h"
#include "doukutsu/inventory.h"

namespace custom_tsc_cmds
{

static bool isCommand(const char* cmd)
{
	const char* addr = &csvanilla::gTS.data[csvanilla::gTS.p_read];
	return addr[1] == cmd[0] && addr[2] == cmd[1] && addr[3] == cmd[2];
}

void maxAllWeapons();

// Implement your custom TSC commands here :)
// Return true if a command was processed (i.e. we should skip to the
// end of the if/else chain in the vanilla parser),
// or false otherwise (i.e. continue checking for vanilla commnds).
bool checkCustomCommands()
{
	using namespace csvanilla;

	if (isCommand("MIM"))
	{
		*MIM::mimFlag = GetTextScriptNo(gTS.p_read + 4);
		gTS.p_read += 8;
	}
	// Just screwing around here
	else if (isCommand("MAE")) // Max Arms Energy - set all weapons to L3 max
	{
		maxAllWeapons();
		gTS.p_read += 4;
	}
	else if (isCommand("ML+")) // You can override existing commands too
	{
		int hp = GetTextScriptNo(gTS.p_read + 4);
		AddMaxLifeMyChar(hp);
		// Refill player's health
		gMC.life = gMC.max_life;
		gTS.p_read += 8;
	}
	else
		return false; // return false if not any of these commands

	return true;
}

void maxAllWeapons()
{
	using csvanilla::gArmsData;
	for (int i = 0; i < 8 && gArmsData[i].code != 0; ++i)
	{
		gArmsData[i].level = 3;
		gArmsData[i].exp = csvanilla::gArmsLevelTable[gArmsData[i].code].exp[2];
	}
}

}
