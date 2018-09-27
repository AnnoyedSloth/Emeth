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

	bool isHealing;
	float healingTime;

	bool isInZoom;
	
	UPROPERTY(EditDefaultsOnly, Category = Skill)
	bool isHiding;

public:
	AMyPlayer();

	virtual void PostInitializeComponents() override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser) override;

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

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int32 GetCurWeapon() const {return Inventory.IndexOfByKey(CurrentWeapon);}

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void MoveForward(float speed);
	void MoveRight(float speed);

	void TurnVertical(float rate);
	void TurnHorizontal(float rate);

	void Walk();
	void Run();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Tumble();

	UFUNCTION(BlueprintCallable, Category = "Transformation")
	void LineTraceTeleport();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void Hide();

	UFUNCTION(BlueprintCallable, Category = "MouseAction")
		void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "MouseAction")
		void ZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Math")
		FRotator SetYToZero(FRotator rotation) const { rotation.Pitch = 0; return rotation; }

	void TakeDamageSelf();

	void OnAttack();
	void OnFire();
	void ThrowBomb();

	virtual void Jump() override;

public:


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EquipWeapon(class AWeapon* weapon);
	virtual void SpawnDefaultInventory();
	virtual void OnChangeWeapon();

	void OnDagger();
	void OnJavelin();
	void OnBomb();
	void EnergyDown(float value);

	virtual void SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon);

	void SetHealingTrue();
	void SetHealingFalse();
};