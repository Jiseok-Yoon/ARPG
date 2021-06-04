#include "PoolableParticle.h"
#include "Particles/ParticleSystemComponent.h"

APoolableParticle::APoolableParticle()
{
	INITIALIZE_OBJECT_POOLABLE(NOT_USE_INSTANCE_ID);
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE_SYSTEM"));
	SetRootComponent(ParticleSystem);
	UE_LOG(LogTemp, Warning, TEXT("PoolableParticle Created"))
}

void APoolableParticle::BeginPlay()
{
	Super::BeginPlay();
	
	ParticleSystem->OnSystemFinished.AddDynamic(this, &APoolableParticle::OnParticleSystemFinished);
}

void APoolableParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoolableParticle::OnRecycleStart()
{
}

void APoolableParticle::OnParticleSystemFinished(UParticleSystemComponent* pSystem)
{
	SetCanRecyclable(true);
}

