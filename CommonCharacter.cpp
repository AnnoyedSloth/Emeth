// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonCharacter.h"
#include "Weapon.h"
#include "Projectile.h"
#include "JsonManager.h"
#include "Engine.h"


// Sets default values
ACommonCharacter::ACommonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = 100.0f;
	//ProjectileClass.GetDefaultObject()->SetOwningPawn(this);
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();

	TSharedPtr<ObjectInfo> myInfo = MakeShareable(new ObjectInfo);
	myInfo = JsonManager::GetInstance()->Load(GetName());
	if (myInfo.IsValid())
	{
		SetActorLocationAndRotation(myInfo->loc, myInfo->rot, false);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myInfo->rot.ToString());
	}
}

void ACommonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName ACommonCharacter::GetWeaponAttachPoint() const
{
	return weaponAttachPoint;
}

void ACommonCharacter::AddWeapon(class AWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon);
	}
}

void ACommonCharacter::SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon)
{
	AWeapon* LocalLastWeapon = NULL;

	if (lastWeapon != NULL)
	{
		LocalLastWeapon = lastWeapon;
	}
	else if (newWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->UnEquip();
	}

	CurrentWeapon = newWeapon;

	if (newWeapon)
	{
		newWeapon->SetOwningPawn(this);
		newWeapon->OnEquip(lastWeapon);
	}
}

void ACommonCharacter::EquipWeapon(class AWeapon* weapon)
{
	if (weapon)
	{
		SetCurrentWeapon(weapon, CurrentWeapon);
	}
}

void ACommonCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClasses.Num();

	for (int32 i = 0; i < NumWeaponClasses; ++i)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;

			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void ACommonCharacter::OnChangeWeapon()
{
	const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);

	AWeapon* NextWeapon = Inventory[(CurrentWeaponIndex) % Inventory.Num()];

	EquipWeapon(NextWeapon);
}


void ACommonCharacter::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BehitAnim);

	if (DamageTaken > 0.0f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}


	JsonManager::GetInstance()->Save(GetName(), GetActorLocation(), GetActorRotation());
}

void ACommonCharacter::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser)
{
	//범위내 작은값을 Health값으로
	Health = FMath::Min(0.0f, Health);


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
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACommonCharacter::OnDieAnimationEnd, playDuration-.2f, false);

}

void ACommonCharacter::OnDieAnimationEnd()
{
	CurrentWeapon->SelfDestroy();
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

float ACommonCharacter::TakeDamage(float Damage, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health < -0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;
	}

	if (Health <= 0)
	{
		Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActualDamage;

}

void ACommonCharacter::OnAttack()
{
	if(CurrentWeapon && !(CurrentWeapon->GetAttackStatus())) CurrentWeapon->StartAttack();
	if (CurrentWeapon == Inventory[1] && !(CurrentWeapon->GetAttackStatus()))
	{
		FTimerHandle AttackHandle;
		GetWorldTimerManager().SetTimer(AttackHandle, this, &ACommonCharacter::OnFire, .5f, false);
	}

}

void ACommonCharacter::OnFire()
{
	if (DefaultProjectileClasses[0] != NULL)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<AProjectile>(DefaultProjectileClasses[0], SpawnLocation, SpawnRotation);
		}

	}
}

void ACommonCharacter::SetAttackStatus(bool isAttack)
{
	isDuringAttack = isAttack;
}