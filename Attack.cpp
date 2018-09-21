// Fill out your copyright notice in the Description page of Project Settings.

#include "Attack.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AICon = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		AICon->AttackEnemy();

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}


