// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "CommonCharacter.h"
#include "MyPlayer.h"
#include "Engine.h"


// Sets default values
AWeapon::AWeapon(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Weapon"));
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	weaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	weaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	weaponCollision->SetVisibility(true);
	weaponCollision->Activate(true);
	weaponCollision->SetupAttachment(WeaponMesh);

	duringAttack = false;
}

//AWeapon::~AWeapon()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Weapon Destroy Called!");
//	Destroy(WeaponMesh);
//	Destroy(WeaponCollision);
//}

void AWeapon::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!owningPawn) return;

	if (duringAttack)
	{
		TArray<AActor*> overlapping;
		if (!weaponCollision) return;

		weaponCollision->GetOverlappingActors(overlapping, TSubclassOf<AActor>());

		for (AActor* actor : overlapping)
		{
			if (!actor) continue;
			if (actor != owningPawn && !actor->ActorHasTag(owningPawn->Tags[0]))
			{
				UGameplayStatics::ApplyDamage(actor, damage, NULL, this, UDamageType::StaticClass());
				duringAttack = false;
			}
		}
	}
}

void AWeapon::SelfDestroy()
{
	Destroy(this);
}

void AWeapon::SetOwningPawn(ACommonCharacter* newOwner)
{
	if (owningPawn != newOwner)
	{
		owningPawn = newOwner;
	}
}

void AWeapon::AttachMeshToPawn()
{
	if (owningPawn)
	{
		USkeletalMeshComponent* PawnMesh = owningPawn->GetMesh();
		FName AttachPoint = owningPawn->GetWeaponAttachPoint();

		//WeaponMesh->AttachTo(PawnMesh, AttachPoint);
		WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
	}
}

void AWeapon::OnEquip(const AWeapon* lastWeapon)
{
	AttachMeshToPawn();
}

void AWeapon::UnEquip()
{
	//WeaponMesh->DetachFromParent();
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}

void AWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Super::NotifyActorBeginOverlap(OtherActor);

	//if (OtherActor->IsA(ACommonCharacter::StaticClass()) && OtherActor != owningPawn && duringAttack && !OtherActor->ActorHasTag(owningPawn->Tags[0]))
	//{
	//	UGameplayStatics::ApplyDamage(OtherActor, damage, NULL, this, UDamageType::StaticClass());

		//duringAttack = false;

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "ApplyDamage");
	//}
}

void AWeapon::StartAttack()
{
	if (!isAttackStarted)
	{
		UAnimInstance* AnimInstance = owningPawn->GetMesh()->GetAnimInstance();

		randomAnim = FMath::Rand()%AttackAnim.Num();

		AnimInstance->Montage_Play(AttackAnim[randomAnim]);
		//duringAttack = true;

		isAttackStarted = true;
		owningPawn->SetAttackStatus(true);

		FTimerHandle AttackHandle;
		GetWorldTimerManager().SetTimer(AttackHandle, this, &AWeapon::SetAttackStatus, startTime, false);

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AWeapon::StopAttack, endTime, false);
	}
}

void AWeapon::StopAttack()
{
	if (!owningPawn) return;
	UAnimInstance* AnimInstance = owningPawn->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Stop(1.0f, AttackAnim[randomAnim]);
	duringAttack = false;
	isAttackStarted = false;
	owningPawn->SetAttackStatus(false);
}

void AWeapon::SetAttackStatus()
{
	duringAttack = true;

}