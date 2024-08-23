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

#pragma once

namespace debug_tsc
{
bool applyPatch();
}
