// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "CommonCharacter.h"
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
	InitialLifeSpan = 3.0f;
	damage = 30.0f;
	instigatorName = "Player";

}

//void AProjectile::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//}


void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
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

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Collision Projectile!!");
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		UGameplayStatics::ApplyPointDamage(Hit.Actor.Get(), 100, -Hit.ImpactNormal, Hit, NULL, this, UDamageType::StaticClass());
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AProjectile::SetOwningPawn(ACommonCharacter* owner)
{
	if (myPawn != owner)
	{
		myPawn = owner;
	}
}