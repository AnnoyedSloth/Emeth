// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Enemy.h"
#include "MyPlayer.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));

	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";

	CurrentPatrolPoint = 0;

}

void AEnemyAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AEnemy* enemyCharacter = Cast<AEnemy>(Pawn);

	if (enemyCharacter && enemyCharacter->EnemyBehavior->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*(enemyCharacter->EnemyBehavior->BlackboardAsset));

		PatrolPoints = enemyCharacter->GetPatrolPoints();

		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);

		BehaviorComp->StartTree(*enemyCharacter->EnemyBehavior);
	}
}

void AEnemyAIController::SetPlayerCaught(APawn* Pawn)
{
	AMyPlayer* enemyCharacter = Cast<AMyPlayer>(Pawn);
	if (BlackboardComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, BehaviorComp->GetActiveNode()->GetNodeName());		
		BlackboardComp->SetValueAsObject(PlayerKey, Pawn);
		//BehaviorComp->GetActiveNode()->
	}
}

void AEnemyAIController::AttackEnemy()
{
	AEnemy* enemyCharacter = Cast<AEnemy>(GetPawn());

	if (enemyCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Attack Activated");
		enemyCharacter->OnAttack();
	}
}