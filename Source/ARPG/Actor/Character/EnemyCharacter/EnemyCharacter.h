#pragma once

#include "ARPG.h"
#include "Actor/Character/BaseCharacter.h"
#include "Struct/EnemyData/EnemyData.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ARPG_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private :
	class UDataTable* DT_EnemyData;
	TSubclassOf<class UEnemyCharacterWidget> CharacterWidgetClass;

private :
	class UBehaviorTree* BehaviorTree;

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCharacterWidgetComponent* CharacterWidget;

public :
	// 적 캐릭터 코드를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Code")
	FName EnemyCode;

	// 적 캐릭터 정보를 나타냅니다.
	FEnemyData * EnemyData;

public :
	AEnemyCharacter();

public :
	virtual void PossessedBy(AController* NewController) override;

public :
	// 적 캐릭터를 초기화합니다.
	/// - 이 메서드는 BeginPlay() 가 호출될 때 호출됩니다.
	void InitializeEnemyCharacter(FName enemyCode);

public :
	FORCEINLINE class UCharacterWidgetComponent* GetCharacterWidget() const
	{ return CharacterWidget; }
	


	
};
