// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticlePlay.h"
#include "Emeth.h"
#include "CommonCharacter.h"
#include "Components/SphereComponent.h"


// Sets default values
AParticlePlay::AParticlePlay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	explosionFX = CreateDefaultSubobject<UParticleSystem>(TEXT("Particle"));

}

void AParticlePlay::BeginPlay()
{
	Super::BeginPlay();

	if (explosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, explosionFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
	
}