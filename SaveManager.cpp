// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"
#include "JsonManager.h"
#include "CommonCharacter.h"
#include "Engine.h"


// Sets default values
ASaveManager::ASaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASaveManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASaveManager::SaveAll()
{
	JsonManager::GetInstance()->ClearMap();
	for (ACommonCharacter* obj : objs)
	{
		JsonManager::GetInstance()->SendData(obj->GetName(), obj->GetActorLocation(), obj->GetActorRotation());
	}

	JsonManager::GetInstance()->Save();
}

void ASaveManager::LoadAll()
{
	TArray<FString> names;

	for (ACommonCharacter* obj : objs)
	{
		names.Add(obj->GetName());
	}

	JsonManager::GetInstance()->LoadInit(names);

	for (ACommonCharacter* obj : objs)
	{
		if (!obj) continue;
		obj->LoadObjData();
	}
}

void ASaveManager::SetSaveManager()
{
	for (ACommonCharacter* obj : objs)
	{
		obj->SetManager(this);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, obj->GetName());
	}
}

void ASaveManager::PushObj(ACommonCharacter* obj)
{
	objs.AddUnique(obj);
}

void ASaveManager::RemoveObj(ACommonCharacter* obj)
{
	objs.Remove(obj);
}