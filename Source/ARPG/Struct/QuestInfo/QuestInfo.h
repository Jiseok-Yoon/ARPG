#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Enum/CharacterClassType.h"
#include "Enum/QuestType.h"

#include "Struct/QuestSubInfo/QuestSubInfo.h"
#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "QuestInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FQuestInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public :
	// 퀘스트 시작 가능 캐릭터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "조건")
	ECharacterClassType QuestAvailableClass;

	// 퀘스트 시작 가능 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "조건")
	int32 QuestAvailableLevel;

	// 선행 퀘스트 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "조건")
	TArray<FName> PrecedentQuestList;
	
	// 퀘스트 타이틀 내용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FText QuestTitleText;
	
	// 퀘스트 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본", meta = (MultiLine = "true"))
	FText QuestDescriptionText;
	
	// 퀘스트 타입 : 메인 / 서브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	EQuestType QuestType;

	// 퀘스트 시작 전 표시될 Npc 대화 내용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FNpcDialogInfo BeforQuestDialog;

	// 퀘스트 시작 후 표시될 Npc 대화 내용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FNpcDialogInfo AfterQuestDialog;

	// 퀘스트 완료 시 표시될 Npc 대화 내용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FNpcDialogInfo CompleteQuestDialog;
	
	// 퀘스트 내용 FQuestSubInfo
	// 실제 퀘스트 내용 데이터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	TArray<FQuestSubInfo> QuestSubInfo;
	
	// 퀘스트 보상 아이템 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "보상")
	TArray<FName> RewardItems;
	
	// 퀘스트 보상 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "보상")
	int32 RewardExp;

	// 퀘스트 보상 은화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "보상")
	int32 REwardSilver;

public:
	// 현재 진행중인 퀘스트인지 확인합니다.
	static bool IsProgress(class UPlayerManager* playerManager, FName questCode);

	// 수주 가능함을 확인합니다.
	bool IsOrderable(class UPlayerManager* playerManager, FName questCode);

};
