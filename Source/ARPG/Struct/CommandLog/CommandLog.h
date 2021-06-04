#pragma once

#include "CoreMinimal.h"
#include "Enum/CommandLogType.h"
#include "CommandLog.generated.h"

USTRUCT()
struct ARPG_API FCommandLog
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	TArray<FString> CommandString;

	UPROPERTY()
	TArray<uint8> CommandLogType;

public :
	void AddLog(FString commandString, ECommandLog::Type commandLogType);

	FORCEINLINE int32 Num() const
	{ return CommandString.Num(); }

	FORCEINLINE TTuple<FString, ECommandLog::Type> operator[](int32 index) const
	{ return MakeTuple(CommandString[index], static_cast<ECommandLog::Type>(CommandLogType[index])); }



};
