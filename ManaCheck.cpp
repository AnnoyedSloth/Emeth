// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaCheck.h"
#include "BossAIController.h"

EBTNodeResult::Type UManaCheck::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AICon = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{

		if (AICon->CheckMana()) return EBTNodeResult::Succeeded;
		else return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}


