#include <string>
#include <string_view>
#include <format>
#include <algorithm>
#include <iterator>

#include "doukutsu/camera.h"
#include "doukutsu/npc.h"

namespace debug_tsc
{
void reportProblem(const std::string& problemText);

// Technically CS already has GetNpCharAlive() which does the same thing...but it's okay, we'll just do it ourselves :)
bool doesNPCExist(int eventNum)
{
	using csvanilla::gNPC;
	return std::any_of(std::begin(gNPC), std::end(gNPC),
		[eventNum](const csvanilla::NPCHAR& npc)
		{
			return (npc.cond & 0x80) != 0 && npc.code_event == eventNum;
		});
}

// <FON
void SetFrameTargetNpChar(int event, int wait)
{
	// For some reason this function doesn't check if the NPC is actually alive
	if (std::none_of(std::begin(csvanilla::gNPC), std::end(csvanilla::gNPC),
		[event](const csvanilla::NPCHAR& npc)
		{
			return npc.code_event == event;
		}))
	{
		reportProblem(std::format("Attempting to <FON on NPC event {}, but no such NPC exists!\n"
			"Make sure that you are passing the correct event number (and not the NPC type) to <FON.", event));
	}

	return csvanilla::SetFrameTargetNpChar(event, wait);
}

constexpr std::string_view TemplateMsg =
"Attempting to {0} on NPC event {1}, but no such NPC exists!\n"
"Make sure that you are passing the correct event number (and not the NPC type) to {0}, "
"and that the NPC you are trying to {0} has actually spawned.";

// <CNP
void ChangeNpCharByEvent(int eventNum, int npcType, int direction)
{
	if (!doesNPCExist(eventNum))
		reportProblem(std::format(TemplateMsg, "<CNP", eventNum));

	return csvanilla::ChangeNpCharByEvent(eventNum, npcType, direction);
}

// <INP
void ChangeCheckableNpCharByEvent(int eventNum, int npcType, int direction)
{
	if (!doesNPCExist(eventNum))
		reportProblem(std::format(TemplateMsg, "<INP", eventNum));

	return csvanilla::ChangeCheckableNpCharByEvent(eventNum, npcType, direction);
}

// <ANP
void SetNpCharActionNo(int eventNum, int actNo, int direction)
{
	if (!doesNPCExist(eventNum))
		reportProblem(std::format(TemplateMsg, "<ANP", eventNum));

	return csvanilla::SetNpCharActionNo(eventNum, actNo, direction);
}

// <MNP
void MoveNpChar(int eventNum, int x, int y, int direction)
{
	if (!doesNPCExist(eventNum))
		reportProblem(std::format(TemplateMsg, "<MNP", eventNum));

	return csvanilla::MoveNpChar(eventNum, x, y, direction);
}

} // namespace debug_tsc
