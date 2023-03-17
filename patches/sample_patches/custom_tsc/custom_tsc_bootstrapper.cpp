// An example of modding via patching new functionality into existing vanilla ASM
// (The custom commands are implemented in custom_tsc_cmds.cpp)

#include "custom_tsc.h"

#include "patch_utils.h"

namespace custom_tsc_cmds
{
using patcher::byte;
using patcher::dword;

// This function will be inserted at the beginning of the command parser
// (optimizing the start of the <END command)
bool checkCustomCommands();
// We'll implement this function in custom_tsc_cmds.cpp. Let's see how we implement the patcher:

void patchTSCHook()
{
	dword address = 0x4225ED; // Start of <END code
	// Extra bytes shaved off by optimizing the ASM
	patcher::writeNOPs(address, 19);
	address += 19;

	// Write the call to our function
	patcher::writeCall(address, checkCustomCommands);
	address += 5; // CALL instruction is 5 bytes

	// Handle the aftermath and replace the <END code that we clobbered (in ASM)
	byte patch2[] = {
		0x84, 0xC0,                         // TEST al, al (test return value)
		0x74, 0x05,                         // JE short 42260E (if false, jump to optimized <END code)
		0xE9, 0x99, 0x2C, 0x00, 0x00,       // JMP 4252A7 (jump to end of command parsing loop)
		// Optimized <END code (not like super optimized, but it doesn't need to be)
		0xA1, 0xD8, 0x5A, 0x4A, 0x00,       // MOV eax, dword ptr [4A5AD8] (gTS.data)
		0x03, 0x05, 0xE0, 0x5A, 0x4A, 0x00, // ADD eax, dword ptr [4A5AE0] (gTS.p_read)
		0x80, 0x78, 0x01, 0x45,             // CMP byte ptr [eax+1], 45 ('E')
		0x75, 0x47,                         // JNE SHORT 422666
		0x80, 0x78, 0x02, 0x4E,             // CMP byte ptr [eax+2], 4E ('N')
		0x75, 0x41,                         // JNE SHORT 422666
		0x80, 0x78, 0x03, 0x44              // CMP byte ptr [eax+3], 44 ('D')
	};
	patcher::patchBytes(address, patch2, sizeof patch2);
}

}