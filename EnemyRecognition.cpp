// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyRecognition.h"
#include "BossAIController.h"
#include "Engine.h"




EBTNodeResult::Type UEnemyRecognition::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AICon = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		if (AICon->GetRecognize())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Failed");
			return EBTNodeResult::Failed;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Succeed");
		return EBTNodeResult::Succeeded;

	}

	return EBTNodeResult::Failed;
}