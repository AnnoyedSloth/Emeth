// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "BossAIController.generated.h"

/**
*
*/
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS(config = game)
class EMETH_API ABossAIController : public AAIController
{
	GENERATED_BODY()

		UPROPERTY(transient)
		UBlackboardComponent* BlackboardComp;
	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName LocationToGoKey;
	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName PlayerKey;

	UPROPERTY(VisibleAnywhere, Category = AI)
		bool isRecog;

	virtual void Possess(APawn* Pawn) override;

public:
	ABossAIController(const class FObjectInitializer& ObjectInitializer);

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	UFUNCTION(BlueprintCallable, Category = AI)
		void SetRecognize(bool recog);

	UFUNCTION(BlueprintCallable, Category = AI)
		bool GetRecognize() const { return isRecog; }

	UFUNCTION(BlueprintCallable, Category = AI)
		virtual void SetPlayerCaught(APawn* myPawn);

	UFUNCTION(BlueprintCallable, Category = AI)
		void Attack();

	UFUNCTION(BlueprintCallable, Category = AI)
		void FireProjectile();

	UFUNCTION(BlueprintCallable, Category = AI)
		bool CheckMana();
};
