// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "BombProjectile.generated.h"

/**
 * 
 */
UCLASS()
class EMETH_API ABombProjectile : public AProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		TSubclassOf<class AParticlePlay> explosion;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	class UStaticMeshComponent* staticSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* sphereCollision;

	UPROPERTY(EditAnywhere, Category = Parameter)
		float explosionDelay;

	UPROPERTY(EditDefaultsOnly, Category = Parameter)
		float impactRadius;

	UFUNCTION()
		void BombExplosion();

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

public:
	ABombProjectile(const class FObjectInitializer& ObjectInitializer);
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UFUNCTION()
		virtual void ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//UFUNCTION()
	//	virtual void OnOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	//UFUNCTION()
	//	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
