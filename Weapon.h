// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class EMETH_API AWeapon : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* WeaponCollision;

	int randomAnim = 0;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		class ACommonCharacter* owningPawn;

	UPROPERTY(EditAnywhere, Category = Animation)
		TArray<UAnimMontage*> AttackAnim;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool duringAttack;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		bool isAttackStarted;

	UPROPERTY(EditAnywhere, Category = Damage)
		float damage;
public:



	// Sets default values for this actor's properties
	AWeapon(const class FObjectInitializer& ObjectInitializer);
	void SetOwningPawn(ACommonCharacter* newOwner);

	UFUNCTION(BlueprintCallable, Category = Pawn)
	ACommonCharacter *GetOwningPawn() const { return owningPawn; }

	void AttachMeshToPawn();


	UFUNCTION()
		void OnEquip(const AWeapon* lastWeapon);
	UFUNCTION()
		void UnEquip();

	UFUNCTION()
		virtual void SetAttackStatus();

	UFUNCTION()
		void StartAttack();
	UFUNCTION()
		void StopAttack();

	UFUNCTION()
		void SelfDestroy();

	UFUNCTION(BlueprintCallable, Category = Status)
		virtual bool GetAttackStatus() const { return duringAttack; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
