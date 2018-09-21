// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCloseAttack.h"
#include "BossAIController.h"

EBTNodeResult::Type UBossCloseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* AICon = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		AICon->Attack();

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
