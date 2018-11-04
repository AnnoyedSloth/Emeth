// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Weapon.h"
#include "EnemyAIController.h"
#include "AIPatrolPoint.h"
#include "MyPlayer.h"
#include "Projectile.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"


// Sets default values
AEnemy::AEnemy()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(50.0f);

	bUseControllerRotationYaw = true;
	DuringAttack = false;

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	AIControllerClass = AEnemyAIController::StaticClass();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnPlayerCaught);
	}

	isSaw = false;
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnDefaultInventory();
}

void AEnemy::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);


	//if (Other->IsA(AEnemy::StaticClass()))
	//{
	//	UGameplayStatics::ApplyDamage(Other, 10.0f, NULL, this, UDamageType::StaticClass());
	//}
}

void AEnemy::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	Super::OnHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);

	AWeapon *causerWeapon = Cast<AWeapon>(DamageCauser);
	AProjectile *causerProjectile = Cast<AProjectile>(DamageCauser);
	
	if (causerWeapon) OnPlayerCaught(causerWeapon->GetOwningPawn());
	if (causerProjectile) OnPlayerCaught(causerProjectile->GetOwningPawn());

	//OnPlayerCaught(PawnInstigator);
	//if(DamageCauser) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, DamageCauser->GetName());

}

void AEnemy::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void AEnemy::PlayMeleeAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(MeleeAnim);

	FTimerHandle AttackHandle;
	GetWorldTimerManager().SetTimer(AttackHandle, this, &AEnemy::SetAttackStatus, 1.8f, false);

	FTimerHandle TimerHandle_EnemyStopAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_EnemyStopAttack, this, &AEnemy::StopMeleeAnim, 2.0f, false);
}

void AEnemy::StopMeleeAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(1.0f, MeleeAnim);
	DuringAttack = false;
}

void AEnemy::SetAttackStatus()
{
	DuringAttack = true;
}

void AEnemy::OnPlayerCaught(APawn* Pawn)
{
	if (isSaw) return;

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	AMyPlayer* myPlayer = Cast<AMyPlayer>(Pawn);

	if (myPlayer->GetHide()) return;

	//if (myPlayer) GEngine->AddOnScreenDebugMessage(-1, 2.0f,FColor::Green, "Cast Success");
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,myPlayer->GetName());
	if (AIController && myPlayer)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Caught!"));
		AIController->SetPlayerCaught(myPlayer);
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		
	}
	isSaw = true;
	//delete AIController;
	//delete myPlayer;
}

void AEnemy::PrintStatus()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "It's OK!");
}

void AEnemy::OnAttack()
{
	if (CurrentWeapon && !(CurrentWeapon->GetAttackStatus())) CurrentWeapon->StartAttack();
}

void AEnemy::LoadObjData()
{
	Super::LoadObjData();

	isSaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}