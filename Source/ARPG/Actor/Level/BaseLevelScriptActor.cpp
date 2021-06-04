#include "BaseLevelScriptActor.h"

#include "Actor/PoolableParticle/PoolableParticle.h"

ABaseLevelScriptActor::ABaseLevelScriptActor()
{
	PoolableParticleClass = APoolableParticle::StaticClass();
}

void ABaseLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	ParticlePool = NewObject<UObjectPool>();
}

APoolableParticle* ABaseLevelScriptActor::CreatePoolableParticleActor(class UParticleSystem* particleTemplate, FVector location, FRotator rotation)
{
	APoolableParticle* poolableParticle = ParticlePool->GetRecycledObject<APoolableParticle>();

	APoolableParticle* newPoolableParticle =
		(poolableParticle != nullptr) ?
		poolableParticle :
		GetWorld()->SpawnActor<APoolableParticle>(PoolableParticleClass);

	newPoolableParticle->SetActorLocationAndRotation(location, rotation);

	ParticlePool->RegisterRecyclableObject(newPoolableParticle);

	newPoolableParticle->GetParticleSystem()->SetTemplate(particleTemplate);
	newPoolableParticle->GetParticleSystem()->Activate(true);


	return newPoolableParticle;
}
