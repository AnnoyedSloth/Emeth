// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "Boss.h"
#include "MyPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine.h"

ABossAIController::ABossAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));

	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";
	isRecog = false;

}

void ABossAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	ABoss* bossCharacter = Cast<ABoss>(Pawn);

	if (bossCharacter && bossCharacter->EnemyBehavior->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*(bossCharacter->EnemyBehavior->BlackboardAsset));

		BehaviorComp->StartTree(*bossCharacter->EnemyBehavior);
	}
}

void ABossAIController::SetRecognize(bool recog)
{
	isRecog = true;
}

void ABossAIController::SetPlayerCaught(APawn* myPawn)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Caught!");
	SetRecognize(true);
	AMyPlayer* enemyCharacter = Cast<AMyPlayer>(myPawn);
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, myPawn);
	}
}

void ABossAIController::Attack()
{
	ABoss* bossPawn = Cast<ABoss>(GetPawn());
	if (bossPawn)
	{
		bossPawn->OnAttack();
	}
}

void ABossAIController::FireProjectile()
{
	ABoss* bossPawn = Cast<ABoss>(GetPawn());
	if (bossPawn)
	{
		bossPawn->OnFire();
	}
}

bool ABossAIController::CheckMana()
{
	ABoss* bossPawn = Cast<ABoss>(GetPawn());
	if (bossPawn && bossPawn->GetEnergy() < 30.0f) return false;
	else return true;
}