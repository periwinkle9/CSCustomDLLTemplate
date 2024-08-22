#include <string>
#include <format>

#include "doukutsu/player.h"
#include "doukutsu/tsc.h"

namespace debug_tsc
{
void reportProblem(const std::string& problemText);

// Keeping track of the current event number to aid with diagnostic messages
int currentEventNumber;
// Used to prevent repeat warnings about text outside <MSG for each character
bool isInUnprintedText = false;

// To be called at the end of StartTextScript() and JumpTextScript().
// Handles the error condition where the exact event number was not found.
csvanilla::BOOL handleEventNotFound(int eventNum, csvanilla::BOOL result)
{
	isInUnprintedText = false; // Reset this
	if (!result)
	{
		reportProblem(std::format("Failed to find event {}! The event will now be stopped to prevent further issues.", eventNum));
		csvanilla::StopTextScript();
		csvanilla::gMC.cond &= ~1; // <END resets this but not StopTextScript()
	}
	else
		currentEventNumber = eventNum;
	return result;
}

// To be called in the TSC parser when the current script being parsed appears to be text.
// Checks that a message box is currently active, and that we haven't ran off the end of the event.
// (The return value of the function is a value that was overwriten in the original ASM to make room for this function.)
int checkUnexpectedText()
{
	using csvanilla::gTS;
	// Check if we fell off the end of the event
	if (gTS.data[gTS.p_read] == '\0' || gTS.data[gTS.p_read] == '#')
	{
		reportProblem(std::format("Reached the end of the current event ({}) without running an <END.\n"
			"The event will continue running from this point in the script, but this is probably not what you want.", currentEventNumber));
		isInUnprintedText = (gTS.flags & 1) == 0;
	}
	// Check if we're about to print text to an invisible message box
	else if (!(gTS.flags & 1) && gTS.mode != 0) // Last check is in case of handleEventNotFound() stopping the event
	{
		if (!isInUnprintedText)
		{
			isInUnprintedText = true;
			std::string unprintedText;
			for (int pos = gTS.p_read; gTS.data[pos] != '\0' && gTS.data[pos] != '<' && gTS.data[pos] != '\r'; ++pos)
				unprintedText += gTS.data[pos];
			reportProblem(std::format("Interpreting \"{}\" as text, but no message box is active. "
				"(This is usually a symptom of another TSC error.)",
				unprintedText));
		}
	}
	else
		isInUnprintedText = false;

	return gTS.flags; // Replaces the 'movsx eax, byte ptr [4A5ADD]' instruction that was removed to call this function
}

} // namespace debug_tsc
