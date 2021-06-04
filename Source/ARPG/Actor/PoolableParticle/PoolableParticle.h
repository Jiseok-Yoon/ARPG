#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ObjectPoolable.h"
#include "PoolableParticle.generated.h"

UCLASS()
class ARPG_API APoolableParticle : 
	public AActor,
	public IObjectPoolable
{
	GENERATED_BODY()

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent * ParticleSystem;
	

public:	
	APoolableParticle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

#pragma region Implements IObjectPoolable
	IMPLEMENT_OBJECT_POOLABLE
	DEF_SETID
	virtual void OnRecycleStart() override;
#pragma endregion

	FORCEINLINE class UParticleSystemComponent* GetParticleSystem() const
	{ return ParticleSystem; }

private:
	UFUNCTION()
	void OnParticleSystemFinished(class UParticleSystemComponent* pSystem);

};
