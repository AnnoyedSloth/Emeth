// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class EMETH_API AExplosion : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		class UParticleSystem* explosionFX;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* sphereCollision;

public:	
	// Sets default values for this actor's properties
	AExplosion(const FObjectInitializer& ObjectInitializer);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void NotifyActorBeginOverlap(AActor* OtherActor);
	
	
};
