#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <limits>
#include <cstddef>
#include <cstring>

#include "doukutsu/cstdlib.h"
#include "doukutsu/tsc.h"

using std::size_t;

namespace csvanilla
{
const auto msize = reinterpret_cast<size_t(*)(void*)>(0x48AF9F);
// Copied from doukutsu/window.h to avoid an unnecessary #include <Windows.h>
const auto& gDataPath = *reinterpret_cast<char(*)[260]>(0x49E220);
}

namespace debug_tsc
{
void reportProblem(const std::string& problemText);

void checkAndReallocTSCBuffer(size_t scriptSize)
{
	// Possible FIXME: check against original msize rather than current msize
	// (in case of multiple scripts being too large, those other scripts may not be caught immediately)
	if (scriptSize >= csvanilla::msize(csvanilla::gTS.data))
	{
		reportProblem(std::format(
			"Current TSC file is too big ({} bytes).\nPlease use the tsc_malloc hack to increase the max TSC buffer size.",
			scriptSize));
		// Reallocate the buffer as a temporary measure to prevent immediate problems
		csvanilla::realloc(csvanilla::gTS.data, scriptSize + 1); // +1 for the terminating null character
	}
}

// Check a TSC file for common errors:
// - Out of order event #s
// - Event #s not 4 digits
// - Possible usages of '#' character outside of event numbers
// - Script too big
void analyzeTSC(const std::string& script)
{
	checkAndReallocTSCBuffer(script.size());
	int lastEventNumber = std::numeric_limits<int>::min();

	auto getLine = [&script](std::string::size_type pos)
	{
		auto lastNewline = script.find_last_of('\n', pos);
		auto nextNewline = script.find_first_of('\r', pos);
		return script.substr(lastNewline + 1, nextNewline - (lastNewline + 1));
	};
	auto parseEventNumber = [&script](std::string::size_type pos)
	{
		int place = 1000;
		int value = 0;
		for (int i = 0; i < 4 && pos + i < script.size(); ++i, place /= 10)
			value += (script[pos + i] - '0') * place;
		return value;
	};
	for (auto currentPos = script.find('#'); currentPos < script.size(); currentPos = script.find('#', currentPos + 1))
	{
		// Check for # not at the start of the line (which could indicate # being used in a textbox instead of an event number)
		if (currentPos > 0 && script[currentPos - 1] != '\n')
		{
			reportProblem(std::format("Detected event number not at the start of a line (possible erroneous use of # in a textbox?).\n"
				"If this is truly meant to be an event number, it is recommended to put it at the start of the line.\nOffending line: {}", getLine(currentPos)));
		}

		int eventNum = parseEventNumber(currentPos + 1);

		// Check for event number less than 4 digits
		auto endOfLine = script.find('\r', currentPos);
		if (endOfLine == std::string::npos)
			endOfLine = script.size();
		if (endOfLine - currentPos < 5)
		{
			std::string eventNumText = script.substr(currentPos, endOfLine - currentPos);
			reportProblem(std::format("Detected event number ({}) that's not 4 digits.\n"
				"This will be treated as event number {}, which may cause more problems later.", eventNumText, eventNum));
		}
		else if (eventNum < lastEventNumber)
			reportProblem(std::format("Detected out of order event numbers: event {} needs to be before event {}", eventNum, lastEventNumber));
		else if (eventNum == lastEventNumber)
			reportProblem(std::format("Detected duplicate event number: {} (the second event will never be called)", eventNum));
		//else
		lastEventNumber = eventNum;
	}
}

bool loadTSC(const char* fileName, std::string& out)
{
	std::string path = std::string{csvanilla::gDataPath} + '\\' + fileName;
	{
		std::ifstream ifs{path, std::ios::binary};
		if (!ifs)
			return false;
		std::ostringstream oss;
		oss << ifs.rdbuf();
		out = oss.str();
	}
	csvanilla::EncryptionBinaryData2(reinterpret_cast<unsigned char*>(out.data()), out.size());
	return true;
}

csvanilla::BOOL LoadTextScript2(const char* name)
{
	std::string script;
	if (!loadTSC(name, script))
		return 0;
	analyzeTSC(script);

	using csvanilla::gTS;
	std::memcpy(gTS.data, script.data(), script.size());
	gTS.data[script.size()] = '\0';
	gTS.size = static_cast<int>(script.size());
	strcpy_s(gTS.path, name);
	return 1;
}

csvanilla::BOOL LoadTextScript_Stage(const char* name)
{
	std::string head, body;
	if (!loadTSC("Head.tsc", head) || !loadTSC(name, body))
		return 0;
	head += body;
	analyzeTSC(head);

	using csvanilla::gTS;
	std::memcpy(gTS.data, head.data(), head.size());
	gTS.data[head.size()] = '\0';
	gTS.size = static_cast<int>(head.size());
	strcpy_s(gTS.path, name);
	return 1;
}

} // namespace debug_tsc
