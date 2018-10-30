// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "MyPlayer.generated.h"

UCLASS()
class EMETH_API AMyPlayer : public ACommonCharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* fpsCamera;

	float senV;
	float senH;

	FVector zoomVector;
	FRotator orient;

	//Energy healing variables
	UPROPERTY(VisibleAnywhere, Category = Skill)
		bool isHealing;
	UPROPERTY(VisibleAnywhere, Category = Skill)
		float healingTime;
	UPROPERTY(VisibleAnywhere, Category = Skill)
		bool isTumbling;

	//Zoom flag
	UPROPERTY(VisibleAnywhere, Category = Skill)
		bool isInZoom;

	//Hiding skill flag
	UPROPERTY(VisibleAnywhere, Category = Skill)
		bool isHiding;

public:
	AMyPlayer();

	UFUNCTION(BlueprintCallable, Category = Data)
		void SetSaveManager(ASaveManager* manager);
	UFUNCTION(BlueprintCallable, Category = Data)
		void SaveData();
	UFUNCTION(BlueprintCallable, Category = Data)
		void LoadData();
	UFUNCTION(BlueprintCallable, Category = Data)


	virtual void PostInitializeComponents() override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, Category = Save)
		class ASaveManager* SaveManagerObject = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float walkSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float runSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* walkSound;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* TumbleAnim;

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool GetHide() const { return isHiding; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int32 GetCurWeapon() const { return Inventory.IndexOfByKey(CurrentWeapon); }

	UFUNCTION()
		void OnTumbleEnded(UAnimMontage* montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Move related functions
	void MoveForward(float speed);
	void MoveRight(float speed);

	void TurnVertical(float rate);
	void TurnHorizontal(float rate);

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable, Category = "Move")
		void Walk();
	UFUNCTION(BlueprintCallable, Category = "Move")
		void Run();


	//Basic skills
	UFUNCTION(BlueprintCallable, Category = "AI")
		void Hide();
	UFUNCTION(BlueprintCallable, Category = "MouseAction")
		void ZoomIn();
	UFUNCTION(BlueprintCallable, Category = "MouseAction")
		void ZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Math")
		FRotator SetYToZero(FRotator rotation) const { rotation.Pitch = 0; return rotation; }


	//These functions will used when testing
	UFUNCTION(BlueprintCallable, Category = "Debug")
		void TakeDamageSelf();
	UFUNCTION(BlueprintCallable, Category = "Debug")
		void LineTraceTeleport();


	//Actions which related to battle
	UFUNCTION(BlueprintCallable, Category = "Action")
		void OnAttack();
	UFUNCTION(BlueprintCallable, Category = "Action")
		void OnFire();
	UFUNCTION(BlueprintCallable, Category = "Action")
		void ThrowBomb();
	UFUNCTION(BlueprintCallable, Category = "Action")
		bool Tumble();
	UFUNCTION(BlueprintCallable, Category = "Status")
		bool GetTumbleStatus() const { return isTumbling; }


public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EquipWeapon(class AWeapon* weapon);
	virtual void SpawnDefaultInventory();
	virtual void OnChangeWeapon();
	virtual float TakeDamage(float Damage, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void OnDagger();
	void OnJavelin();
	void OnBomb();
	void EnergyDown(float value, float delay);

	virtual void SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon);
};