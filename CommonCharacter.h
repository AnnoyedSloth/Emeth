// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"

UCLASS()
class EMETH_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//Player status variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		float Energy;

	//Weapon Inventory related variables, functions
		//Weapon Inventory related Arraies, variables
	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<TSubclassOf<class AWeapon>> DefaultInventoryClasses;
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> DefaultProjectileClasses;
	UPROPERTY(EditAnywhere, Category = Inventory)
		class AWeapon* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName weaponAttachPoint;
	TArray<class AWeapon*> Inventory;
	void AddWeapon(class AWeapon* Weapon);
	void SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon);
	virtual void SpawnDefaultInventory();

	//Attack status flag
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		bool isDuringAttack;

	//Animations & Particles & Sounds
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* BehitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* DeathAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		TSubclassOf<class AParticlePlay> bloodParticle;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundCue* hitSound;

	//Dying Event 
	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser);
	virtual void OnDieAnimationEnd();

	//Save Manager
	UPROPERTY(VisibleAnywhere, Category = Data)
		class ASaveManager* saveManager;

public:
	ACommonCharacter();
	
	//Default built-in functions.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Data)
	virtual void LoadObjData();

	UFUNCTION(BlueprintCallable, Category = Data)
		void SetManager(ASaveManager *manager);
	
	//Player attack related functions
	void OnAttack();
	void OnFire();
	void SetAttackStatus(bool isAttack);
	bool GetAttackStatus() const { return isDuringAttack; }

	UFUNCTION()
		float GetEnergy() const { return Energy; }

	FName GetWeaponAttachPoint() const;	
	void EquipWeapon(class AWeapon* weapon);
	void OnChangeWeapon();
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
