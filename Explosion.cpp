// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosion.h"
#include "Emeth.h"
#include "CommonCharacter.h"
#include "Components/SphereComponent.h"


// Sets default values
AExplosion::AExplosion(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	//explosionLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, "ExplosionLight");
	//RootComponent = explosionLight;
	//explosionLight->AttenuationRadius = 400.0;
	//explosionLight->Intensity = 500.0f;
	//explosionLight->bUseInverseSquaredFalloff = false;
	//explosionLight->LightColor = FColor(255, 185, 35);
	//explosionLight->CastShadows = false;
	//explosionLight->bVisible = true;


	PrimaryActorTick.bCanEverTick = true;
	explosionFX = CreateDefaultSubobject<UParticleSystem>(TEXT("Particle"));

}

void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	if (explosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, explosionFX, GetActorLocation(), GetActorRotation());
	}
	
}

void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
