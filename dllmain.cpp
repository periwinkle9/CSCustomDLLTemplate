// dllmain.cpp : Defines the entry point for the DLL application.
// (You shouldn't need to modify anything in this file.)
#include "windows_h_wrapper.h"

bool applyPatches();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    try
    {
        if (ul_reason_for_call == DLL_PROCESS_ATTACH)
            return applyPatches();
    }
    catch (...)
    {
        return FALSE;
    }

    return TRUE;
}
