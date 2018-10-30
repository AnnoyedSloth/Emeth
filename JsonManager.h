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
	FString tag;
};

class EMETH_API JsonManager
{
private:
	JsonManager();
	static JsonManager *instance;

	TMap<FString, TSharedPtr<FJsonObject>> *saveMap;
	TMap<FString, TSharedPtr<FJsonObject>> *loadMap;
	TArray<FString> myArr;

	FCriticalSection criticalSection;

	const FString filePath = FPaths::GameDir() + TEXT("/Content/Json/myJson.json");

public:
	~JsonManager();

	static JsonManager *GetInstance();
	void Save();
	void ClearMap();
	void SendData(FString name, FVector loc, FRotator rot);
	void LoadInit(TArray<FString> objs);
	TSharedPtr<ObjectInfo> Load(FString name);
};
