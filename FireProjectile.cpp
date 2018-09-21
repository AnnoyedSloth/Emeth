// Fill out your copyright notice in the Description page of Project Settings.

#include "FireProjectile.h"
#include "BossAIController.h"
#include "Engine.h"


EBTNodeResult::Type UFireProjectile::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AICon = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{

		AICon->FireProjectile();
		return EBTNodeResult::Succeeded;

	}

	return EBTNodeResult::Failed;
}