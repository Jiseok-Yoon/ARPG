#include "CommandLog.h"

void FCommandLog::AddLog(FString commandString, ECommandLog::Type commandLogType)
{
	CommandString.Add(commandString);
	CommandLogType.Add(commandLogType);
}
