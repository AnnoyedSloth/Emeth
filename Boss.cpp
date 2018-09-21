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
	//범위내 작은값을 Health값으로
	Health = FMath::Min(0.0f, Health);

	//Destroy(CurrentWeapon);

	// 데미지타입값 작성. 데미지타입 클래스가 정의시 사용 미정의시 디폴트값 사용
	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	// 오브젝트 시간과 관계있는 값
	//GetWorldTimerManager().ClearAllTimersForObject(this);

	// 콜리전 해제
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	// 이동불가
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	// 컨트롤러 해제
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


