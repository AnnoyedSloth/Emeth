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


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	class ACommonCharacter* MyPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool duringAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	bool isAttackStarted;

	// Sets default values for this actor's properties
	AWeapon(const class FObjectInitializer& ObjectInitializer);
	void SetOwningPawn(ACommonCharacter* newOwner);

	void AttachMeshToPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float damage;

	UFUNCTION()
	void OnEquip(const AWeapon* lastWeapon);
	UFUNCTION()
	void UnEquip();

	void SetAttackStatus();

	UFUNCTION()
	void StartAttack();
	UFUNCTION()
	void StopAttack();

	UFUNCTION()
		void SelfDestroy();

	bool GetAttackStatus() const { return duringAttack; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
