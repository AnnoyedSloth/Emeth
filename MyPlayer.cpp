// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Engine.h"
#include "ParticlePlay.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "JsonManager.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;
	Energy = 100.0f;

	senV = 45.f;
	senH = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	isHiding = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = walkSpeed;
	CameraBoom->bUsePawnControlRotation = true;

	fpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	fpsCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	fpsCamera->RelativeLocation = FVector(0, 0, BaseEyeHeight);
	fpsCamera->bUsePawnControlRotation = true;

	isInZoom = false;

	zoomVector = GetActorLocation() + FVector(5, 0, -10);
	
}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnDefaultInventory();
}


// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isHealing) Energy += 5.0f * DeltaTime;

	if (Energy <= 0)
	{
		Energy = 0;
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	if (Energy >= 100) Energy = 100;

	if (healingTime <= 0)
	{
		healingTime = 0;
		isHealing = true;
	}
	else
	{
		healingTime -= DeltaTime;
		isHealing = false;
	}

	if (GetHide())
	{
		Energy -= 5.0f * DeltaTime;
		isHealing = false;
	}

	if (isInZoom)
	{
		orient = fpsCamera->GetForwardVector().ToOrientationRotator();

		//fpsCamera->SetRelativeLocation(FVector(150, 30, 64), false);
		SetActorRotation(orient);
	}
	else
	{
		//fpsCamera->SetRelativeLocation(FVector(0, 0, 64), false);
	}

}

void AMyPlayer::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Hitted!");
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(BehitAnim);

	if (DamageTaken > 0.0f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}


// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyPlayer::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyPlayer::Walk);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnH", this, &AMyPlayer::TurnHorizontal);
	PlayerInputComponent->BindAxis("TurnV", this, &AMyPlayer::TurnVertical);


	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AMyPlayer::OnChangeWeapon);

	PlayerInputComponent->BindAction("Dagger", IE_Pressed, this, &AMyPlayer::OnDagger);
	PlayerInputComponent->BindAction("Javelin", IE_Pressed, this, &AMyPlayer::OnJavelin);
	PlayerInputComponent->BindAction("Bomb", IE_Pressed, this, &AMyPlayer::OnBomb);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayer::OnAttack);

	PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &AMyPlayer::Hide);

	PlayerInputComponent->BindAction("TakeDamage", IE_Pressed, this, &AMyPlayer::TakeDamageSelf);
	PlayerInputComponent->BindAction("Ray", IE_Pressed, this, &AMyPlayer::LineTraceTeleport);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyPlayer::MoveForward(float value)
{
	if (isDuringAttack) return;
	if ((Controller != NULL) && value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (GetCharacterMovement()->MaxWalkSpeed == runSpeed)
			Energy -= 10.0f * GetWorld()->GetDeltaSeconds();

		//UGameplayStatics::PlaySoundAtLocation(this, walkSound, GetActorLocation());

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);

	}
}

void AMyPlayer::MoveRight(float value)
{
	if (isDuringAttack) return;
	if ((Controller != NULL) && value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (GetCharacterMovement()->MaxWalkSpeed == runSpeed)
			Energy -= 10.0f * GetWorld()->GetDeltaSeconds();

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AMyPlayer::TurnVertical(float rate)
{
	AddControllerPitchInput(rate * senV * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::TurnHorizontal(float rate)
{
	AddControllerYawInput(rate * senH * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

}

void AMyPlayer::Run()
{	
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	healingTime = 1.0f;
}

void AMyPlayer::Jump()
{
	if (isDuringAttack) return;
	Super::Jump();
	healingTime = 1.0f;
}

bool AMyPlayer::Tumble()
{
	if (isTumbling || Energy < 10.0f) return false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;

	EnergyDown(10.0f, 1.0f);
	isTumbling = true;

	AnimInstance->Montage_Play(TumbleAnim);
	EndDelegate.BindUObject(this, &AMyPlayer::OnTumbleEnded);
	
	AnimInstance->Montage_SetEndDelegate(EndDelegate);
	return true;
}

void AMyPlayer::OnTumbleEnded(UAnimMontage* montage, bool bInterrupted)
{
	isTumbling = false;
}

void AMyPlayer::LineTraceTeleport()
{
	FVector fwrDir = fpsCamera->GetForwardVector();
	fwrDir.Z = 0;
	FVector startTrace = GetActorLocation() + FVector(0, 0, 100);
	FVector endTrace = (fwrDir * 1000) + startTrace - FVector(0, 0, 250);

	FHitResult lineHit;
	FCollisionQueryParams CQP;

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "fwrDir : " + fwrDir.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "startTrace : " + startTrace.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "endTrace : " + endTrace.ToString());

	if (GetWorld()->LineTraceSingleByChannel(lineHit, startTrace, endTrace, ECC_Visibility, CQP))
	{
		//TeleportTo(lineHit.Location, fwrDir.Rotation());
		SetActorLocationAndRotation(lineHit.Location + FVector(0,0,20), fwrDir.Rotation());
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "LineHit : " + lineHit.Location.ToString());

	DrawDebugLine(GetWorld(), startTrace, endTrace, FColor(255, 0, 0), true, 5);
	DrawDebugBox(GetWorld(), lineHit.Location, FVector(1,1,1), FColor(255,0,0), false, 5);
}

void AMyPlayer::Hide()
{
	if (!isHiding)
	{
		isHiding = true;
		GetMesh()->SetScalarParameterValueOnMaterials("Opacity", 0.3f);
	}
	else
	{
		isHiding = false;
		GetMesh()->SetScalarParameterValueOnMaterials("Opacity", 1.0f);
		healingTime = 1.0f;
	}
}

void AMyPlayer::EnergyDown(float value, float delay)
{
	Energy -= value;
	healingTime = delay;
}


void AMyPlayer::ZoomIn()
{
	isInZoom = true; 
	fpsCamera->SetRelativeLocation(FVector(150, 30, 64), false);

	if(IsJumpProvidingForce()) GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	else GetCharacterMovement()->SetMovementMode(MOVE_None);

	//SetActorRotation(fpsCamera->GetForwardVector().ToOrientationQuat());
	//SetActorRotation(fpsCamera->GetForwardVector().ToOrientationQuat);
	//fpsCamera->SetRelativeLocation(FVector(150, 30, 64), false);
}

void AMyPlayer::ZoomOut()
{
	orient.Pitch = 0;
	fpsCamera->SetRelativeLocation(FVector(0, 0, 64), false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetActorRotation(orient);
	isInZoom = false;
	//fpsCamera->SetRelativeLocation(FVector(0, 0, 64), false);
}

void AMyPlayer::OnAttack()
{
	if (isTumbling) return;
	if (CurrentWeapon == Inventory[1] && !(CurrentWeapon->GetAttackStatus()))
	{
		if (Energy >= 10.0f)
		{
			EnergyDown(10.0f, 1.0f);
			CurrentWeapon->StartAttack();
			FTimerHandle AttackHandle;
			GetWorldTimerManager().SetTimer(AttackHandle, this, &AMyPlayer::OnFire, .5f, false);
		}
	}
	else if (CurrentWeapon == Inventory[2] && !(CurrentWeapon->GetAttackStatus()))
	{
		if (Energy >= 30.0f)
		{
			EnergyDown(30.0f, 1.0f);
			CurrentWeapon->StartAttack();
			FTimerHandle AttackHandle;
			GetWorldTimerManager().SetTimer(AttackHandle, this, &AMyPlayer::ThrowBomb, .35f, false);
		}
	}
	else if (!(CurrentWeapon->GetAttackStatus()))
	{
		EnergyDown(0, 1.0f);
		CurrentWeapon->StartAttack();
	}
}

void AMyPlayer::OnFire()
{
	if (DefaultProjectileClasses[0] != NULL)
	{
		//AddActorWorldRotation(fpsCamera->GetForwardVector().ToOrientationQuat());
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<AProjectile>(DefaultProjectileClasses[0], SpawnLocation, SpawnRotation);
		}
	}
}

void AMyPlayer::ThrowBomb()
{
	if (isTumbling) return;
	if (DefaultProjectileClasses[1] != NULL)
	{
		//AddActorWorldRotation(fpsCamera->GetForwardVector().ToOrientationQuat());
		const FRotator SpawnRotation = GetActorRotation();		
		//const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));

		const FVector SpawnLocation = GetMesh()->GetSocketLocation(GetWeaponAttachPoint()) + SpawnRotation.RotateVector(FVector(0,0,0));

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<AProjectile>(DefaultProjectileClasses[1], SpawnLocation, SpawnRotation);
		}
	}
}

void AMyPlayer::TakeDamageSelf()
{
	Health -= 10;
}

void AMyPlayer::SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon)
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

void AMyPlayer::EquipWeapon(class AWeapon* weapon)
{
	if (weapon)
	{
		SetCurrentWeapon(weapon, CurrentWeapon);
	}
}

void AMyPlayer::SpawnDefaultInventory()
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

void AMyPlayer::OnChangeWeapon()
{
	const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);

	AWeapon* NextWeapon = Inventory[(CurrentWeaponIndex) % Inventory.Num()];

	EquipWeapon(NextWeapon);
}

void AMyPlayer::OnDagger()
{
	AWeapon* NextWeapon = Inventory[0 % Inventory.Num()];

	EquipWeapon(NextWeapon);
}

void AMyPlayer::OnJavelin()
{
	AWeapon* NextWeapon = Inventory[1 % Inventory.Num()];

	EquipWeapon(NextWeapon);
}

void AMyPlayer::OnBomb()
{
	AWeapon* NextWeapon = Inventory[2 % Inventory.Num()];

	EquipWeapon(NextWeapon);
}

float AMyPlayer::TakeDamage(float Damage, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (isTumbling) return 0;
	if (Health < -0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;

		GetWorld()->SpawnActor<AParticlePlay>(bloodParticle, GetActorLocation(), GetActorRotation());
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