// Fill out your copyright notice in the Description page of Project Settings.

#include "BombProjectile.h"
#include "Emeth.h"
#include "ParticlePlay.h"
#include "CommonCharacter.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Engine.h"

ABombProjectile::ABombProjectile(const class FObjectInitializer& ObjectInitializer) : AProjectile(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = true;
	explosionDelay = 2.5f;

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Targets"));
	sphereCollision->InitSphereRadius(20);
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCollision->SetVisibility(true);
	sphereCollision->Activate(true);
	sphereCollision->SetupAttachment(ProjectileCollision);
}

void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();
	//sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombProjectile::OnOverlapEnter);
	//sphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABombProjectile::OnOverlapEnd);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombProjectile::ProjectileOverlap);
}

void ABombProjectile::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	sphereCollision->SetWorldLocation(this->GetActorLocation());
	explosionDelay -= deltaTime;

	if (explosionDelay < 0)
	{
		BombExplosion();
	}
}

//Must leave as empty function
//Not to execute base class' function
void ABombProjectile::NotifyActorBeginOverlap(AActor* OtherActor){}

void ABombProjectile::BombExplosion()
{
	GetWorld()->SpawnActor<AParticlePlay>(explosion, GetActorLocation(), GetActorRotation());
	TArray<AActor*> overlapping;

	sphereCollision->GetOverlappingActors(overlapping, TSubclassOf<AEnemy>());

	for (AActor* actor : overlapping)
	{
		ADestructibleActor* isDest = Cast<ADestructibleActor>(actor);
		if (isDest)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, isDest->GetFName().ToString());
			isDest->GetDestructibleComponent()->SetSimulatePhysics(true);
			isDest->GetDestructibleComponent()->AddRadialForce(this->GetActorLocation(), 200.0f, 1000000.0f, RIF_Constant);
		}

		UGameplayStatics::ApplyDamage(actor, 100.0f, NULL, this, UDamageType::StaticClass());
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Bomb!");
	Destroy();
}

//Execute when Projectile collision overlapped
void ABombProjectile::ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->Tags.Num() == 0) return;


	//if(OtherActor-> == ECC_WorldStatic)
	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor->Tags[0] != instigatorName)
	{
		BombExplosion();
	}
}

////Execute when Distance collision overlapped
//void ABombProjectile::OnOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
//{
//	AEnemy *myActor = Cast<AEnemy>(OtherActor);
//
//	if (OtherActor->IsA(AEnemy::StaticClass()))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, OtherActor->GetFName().ToString());
//	}
//}
//
//void ABombProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!OtherActor->ActorHasTag("Player"))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Overlap End!");
//	}
//}