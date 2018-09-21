// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"

UCLASS()
class EMETH_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACommonCharacter();
	
	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AWeapon* weapon);
	void OnChangeWeapon();
	void OnAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		float Energy;

	UFUNCTION()
		float GetEnergy() const { return Energy; }


	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TArray<TSubclassOf<class AProjectile>> DefaultProjectileClasses;

	virtual float TakeDamage(float Damage, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<TSubclassOf<class AWeapon>> DefaultInventoryClasses;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Inventory)
	class AWeapon* CurrentWeapon;

	void OnFire();

	void SetAttackStatus(bool isAttack);
	bool GetAttackStatus() const { return isDuringAttack; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName weaponAttachPoint;

	TArray<class AWeapon*> Inventory;


	void AddWeapon(class AWeapon* Weapon);

	void SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon);


	void SpawnDefaultInventory();

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* BehitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* DeathAnim;


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	bool isDuringAttack;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser);
	
	virtual void OnDieAnimationEnd();

};
