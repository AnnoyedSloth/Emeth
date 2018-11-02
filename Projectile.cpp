// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "CommonCharacter.h"
#include "MyPlayer.h"
#include "Engine.h"


AProjectile::AProjectile(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
	ProjectileCollision->CastShadow = true;
	ProjectileCollision->InitSphereRadius(5.0f);
	ProjectileCollision->BodyInstance.SetCollisionProfileName("Projectile");
	//ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	//ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin, STATIC_FUNCTION_FNAME(TEXT("")));
	RootComponent = ProjectileCollision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = ProjectileCollision;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	InitialLifeSpan = 10.0f;
	damage = 30.0f;
	instigatorName = "Player";

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!owningPawn) return;
	Super::NotifyActorBeginOverlap(OtherActor);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, GetOwner()->GetActorLabel());
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, OtherActor->GetOwner()->GetActorLabel());
	if (OtherActor->Tags.Num() == 0) return;
	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor->Tags[0] != instigatorName)
	{
		UGameplayStatics::ApplyDamage(OtherActor, damage, NULL, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "ApplyDamage");
		Destroy();
	}
}

void AProjectile::SetOwningPawn(ACommonCharacter* owner)
{
	if (!owner) return;
	AMyPlayer* player = Cast<AMyPlayer>(owner);

	if(player)
		owningPawn = player;
}