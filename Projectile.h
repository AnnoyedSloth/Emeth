// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class EMETH_API AProjectile : public AActor
{
	GENERATED_BODY()

	class ACommonCharacter *myPawn;
public:
	AProjectile(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		FName instigatorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float damage;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	FORCEINLINE class USphereComponent* GetProjectileCollision() const { return ProjectileCollision; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetOwningPawn(ACommonCharacter *owner);
	ACommonCharacter *GetOwningPawn() const { return myPawn; }
	
};
