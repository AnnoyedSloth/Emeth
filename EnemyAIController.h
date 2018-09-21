// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS(config=game)
class EMETH_API AEnemyAIController : public AAIController
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

	UPROPERTY(EditDefaultsOnly, Category = AI)
		TArray<class AAIPatrolPoint*> PatrolPoints;


	virtual void Possess(APawn* Pawn) override;


public:

	AEnemyAIController(const class FObjectInitializer& ObjectInitializer);

	void SetPlayerCaught(APawn* Pawn);

	void AttackEnemy();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<class AAIPatrolPoint*> GetPatrolPoints() const { return PatrolPoints; }

	int32 CurrentPatrolPoint = 0;
};
