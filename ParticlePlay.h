// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticlePlay.generated.h"

UCLASS()
class EMETH_API AParticlePlay : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		class UParticleSystem* explosionFX;

public:	
	// Sets default values for this actor's properties
	AParticlePlay(const FObjectInitializer& ObjectInitializer);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	
};
