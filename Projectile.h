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

protected:
	class ACommonCharacter *owningPawn = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundCue* collisionSound;

	UPROPERTY(EditAnywhere, Category = Projectile)
		FName instigatorName;

	UPROPERTY(EditAnywhere, Category = Projectile)
		float damage;


public:
	AProjectile(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTIme) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	FORCEINLINE class USphereComponent* GetProjectileCollision() const { return ProjectileCollision; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetOwningPawn(ACommonCharacter *owner);
	ACommonCharacter *GetOwningPawn() const { return owningPawn; }
	
};
