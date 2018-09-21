// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boss.generated.h"

/**
*
*/
UCLASS()
class EMETH_API ABoss : public AEnemy
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TArray<TSubclassOf<class AProjectile>> projectiles;

	virtual void BeginPlay() override;
	virtual void OnPlayerCaught(APawn* Pawn);
	virtual void Tick(float DeltaTime) override;
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser);
public:
	ABoss();


	UFUNCTION()
		void OnFire();

};
