// Fill out your copyright notice in the Description page of Project Settings.

#include "BombProjectile.h"
#include "Emeth.h"
#include "Explosion.h"
#include "CommonCharacter.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Engine.h"

ABombProjectile::ABombProjectile(const class FObjectInitializer& ObjectInitializer) : AProjectile(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = true;
	explosionDelay = 3.0f;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	//if (SphereMeshAsset.Succeeded())
	//{
	//	staticSphere->SetStaticMesh(SphereMeshAsset.Object);
	//}
	//RootComponent = staticSphere;
	

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Targets"));
	sphereCollision->InitSphereRadius(20);
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCollision->SetVisibility(true);
	sphereCollision->Activate(true);
	sphereCollision->SetupAttachment(ProjectileCollision);

	//ProjectileCollision->SetupAttachment(staticSphere);

}

void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();
	sphereCollision->SetWorldLocation(this->GetActorLocation());
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombProjectile::OnOverlapEnter);
	sphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABombProjectile::OnOverlapEnd);
}

void ABombProjectile::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	explosionDelay -= deltaTime;

	if (explosionDelay < 0)
	{
		BombExplosion();
	}
}

void ABombProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Overlaped");
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, OtherActor->GetOwner()->GetActorLabel());
	if (OtherActor->Tags.Num() == 0) return;
	//if(OtherActor-> == ECC_WorldStatic)
	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor->Tags[0] != instigatorName)
	{
		BombExplosion();
	}
}

void ABombProjectile::BombExplosion()
{
	GetWorld()->SpawnActor<AExplosion>(explosion, GetActorLocation(), GetActorRotation());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Bomb!");
	Destroy();
}

void ABombProjectile::OnOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AEnemy *myActor = Cast<AEnemy>(OtherActor);

	if (OtherActor->IsA(ACommonCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, OtherActor->GetFName().ToString());
	}

	//if (myActor)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, OtherActor->GetFName().ToString());
	//}	
}

void ABombProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (!OtherActor->ActorHasTag("Player"))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Overlap End!");
	//}
}