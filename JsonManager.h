// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct ObjectInfo
{
	FVector loc;
	FRotator rot;
};

class EMETH_API JsonManager
{
private:
	JsonManager();
	static JsonManager *instance;

	TMap<FString, TSharedPtr<FJsonObject>> *jsonMap;
	TMap<FString, TSharedPtr<FJsonObject>> *loadMap;
	TArray<FString> myArr;
	bool isDeserialized;

	FCriticalSection criticalSection;

	const FString filePath = FPaths::GameDir() + TEXT("/Content/Json/myJson.json");

public:
	~JsonManager();

	static JsonManager *GetInstance();
	void Save(FString name, FVector loc, FRotator rot);
	void LoadInit();
	TSharedPtr<ObjectInfo> Load(FString name);
};
