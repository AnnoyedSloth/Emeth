// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveManager.generated.h"

UCLASS()
class EMETH_API ASaveManager : public AActor
{
	GENERATED_BODY()

	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Actors)
		TArray<class ACommonCharacter*> objs;
	// Sets default values for this actor's properties
	ASaveManager();

	UFUNCTION(BlueprintCallable, Category = Data)
	void SaveAll();
	UFUNCTION(BlueprintCallable, Category = Data)
	void LoadAll();
	UFUNCTION(BlueprintCallable, Category = Data)
		void SetSaveManager();
	UFUNCTION(BlueprintCallable, Category = Data)
		void PushObj(ACommonCharacter* obj);
	UFUNCTION(BlueprintCallable, Category = Data)
		void RemoveObj(ACommonCharacter* obj);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	
	
};
