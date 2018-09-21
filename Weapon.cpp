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

	duringAttack = false;
}

//AWeapon::~AWeapon()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Weapon Destroy Called!");
//	Destroy(WeaponMesh);
//	Destroy(WeaponCollision);
//}

void AWeapon::SelfDestroy()
{
	Destroy(this);
}

void AWeapon::SetOwningPawn(ACommonCharacter* newOwner)
{
	if (MyPawn != newOwner)
	{
		MyPawn = newOwner;
	}
}

void AWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();

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
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor != MyPawn && duringAttack)
	{
		UGameplayStatics::ApplyDamage(OtherActor, damage, NULL, this, UDamageType::StaticClass());

		duringAttack = false;

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "ApplyDamage");
	}

}

void AWeapon::StartAttack()
{
	if (!isAttackStarted)
	{
		UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();

		randomAnim = FMath::Rand()%AttackAnim.Num();

		AnimInstance->Montage_Play(AttackAnim[randomAnim]);
		//duringAttack = true;

		isAttackStarted = true;
		MyPawn->SetAttackStatus(true);

		FTimerHandle AttackHandle;
		GetWorldTimerManager().SetTimer(AttackHandle, this, &AWeapon::SetAttackStatus, .3f, false);

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AWeapon::StopAttack, 1.0f, false);
	}
}

void AWeapon::StopAttack()
{
	UAnimInstance* AnimInstance = MyPawn->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(1.0f, AttackAnim[randomAnim]);
	duringAttack = false;
	isAttackStarted = false;
	MyPawn->SetAttackStatus(false);
}

void AWeapon::SetAttackStatus()
{
	duringAttack = true;
}