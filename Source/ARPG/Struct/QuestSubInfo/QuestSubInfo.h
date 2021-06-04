#pragma once

#include "CoreMinimal.h"
#include "Enum/SubQuestType.h"
#include "QuestSubInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FQuestSubInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// 퀘스트 서브 타입 : Npc 대화 / 몬스터 사냥 / 아이템 획득
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	ESubQuestType SubQuestType;
	
	// 퀘스트 서브 타이틀
	/// - 이 문자열은 퀘스트 제목 하단에 표시됩니다.
	/// - ex) ~~~ 모으기 (0 / 3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FText QuestSubTitleText;	
	
	// 목표 코드
	/// - Npc 코드, 아이템 코드, 몬스터 코드 ... 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	FName TargetCode;
	
	// 목표 수치 / 금액 / 마리 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본")
	int32 TargetValue;
	
	// 퀘스트 완료 시 수집한 아이템 / 은화를 회수할 것인지를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기타")
	bool bRetrievalCollection;

	// 현재 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기타")
	int32 CurrentValue;
	
	// 완료 상태를 나타냅니다.
	bool bIsComplete;

public :
	FQuestSubInfo();

};
