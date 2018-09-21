// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class EMETH_API AEnemy : public ACommonCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = AI)
		TArray<class AAIPatrolPoint*> PatrolPoints;

public:
	AEnemy();

	virtual void BeginPlay() override;


	virtual void PostInitializeComponents() override;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	void PlayMeleeAnim();
	void StopMeleeAnim();

	void SetAttackStatus();

	void PrintStatus();

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = AI)
		class UBehaviorTree* EnemyBehavior;

	UPROPERTY(VisibleAnywhere, Category = AI)
		class UPawnSensingComponent* PawnSensingComp;
	
	UFUNCTION()
		virtual void OnAttack();

	FORCEINLINE TArray<class AAIPatrolPoint*> GetPatrolPoints() const { return PatrolPoints; }

	UFUNCTION()
		virtual void OnPlayerCaught(APawn* Pawn);


protected:
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* MeleeAnim;

	bool DuringAttack;

};
