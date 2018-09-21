// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "MyPlayer.h"
#include "Projectile.h"
#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"

ABoss::ABoss()
{
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	Health = 500.0f;
	Energy = 100.0f;
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABoss::OnPlayerCaught);
	}
}

void ABoss::OnPlayerCaught(APawn* Pawn)
{
	ABossAIController* AIController = Cast<ABossAIController>(GetController());
	AMyPlayer* myPlayer = Cast<AMyPlayer>(Pawn);
	if (AIController && !AIController->GetRecognize())
	{
		if (myPlayer && !myPlayer->GetHide())
		{
			AIController->SetPlayerCaught(Pawn);
		}
	}
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Energy >= 100) Energy = 100;
	else  Energy += DeltaTime;
}

void ABoss::OnFire()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		Energy -= 30.0f;
		World->SpawnActor<AProjectile>(projectiles[0], SpawnLocation, SpawnRotation);

	}

}

void ABoss::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser)
{
	//������ �������� Health������
	Health = FMath::Min(0.0f, Health);

	//Destroy(CurrentWeapon);

	// ������Ÿ�԰� �ۼ�. ������Ÿ�� Ŭ������ ���ǽ� ��� �����ǽ� ����Ʈ�� ���
	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	// ������Ʈ �ð��� �����ִ� ��
	//GetWorldTimerManager().ClearAllTimersForObject(this);

	// �ݸ��� ����
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	// �̵��Ұ�
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	// ��Ʈ�ѷ� ����
	if (Controller != NULL)
	{
		Controller->UnPossess();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Died!");

	//float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopSlotAnimation();

	float playDuration = AnimInstance->Montage_Play(DeathAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABoss::OnDieAnimationEnd, playDuration - .2f, false);

}


