// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonManager.h"
#include "CommonCharacter.h"
#include <mutex>
#include <JsonSerializer.h>
#include <Json.h>
#include "Engine.h"

JsonManager *JsonManager::instance = nullptr;
static std::once_flag onceFlag;

JsonManager::JsonManager()
{
	saveMap = new TMap<FString, TSharedPtr<FJsonObject>>();
	loadMap = new TMap<FString, TSharedPtr<FJsonObject>>();
	//JsonManager::GetInstance()->LoadInit();
	//LoadInit();
}

JsonManager::~JsonManager()
{
}

JsonManager* JsonManager::GetInstance()
{
	std::call_once(onceFlag, []
	{
		if (instance == nullptr) instance = new JsonManager();
	}
	);
	return instance;
}

void JsonManager::ClearMap()
{
	saveMap->Empty();
}

void JsonManager::SendData(FString name, FVector loc, FRotator rot)
{
	if (saveMap->Find(name) == nullptr)
	{
		TSharedPtr<FJsonObject> objInfo = MakeShareable(new FJsonObject);
		saveMap->Add(name, objInfo);
	}

	TSharedPtr<FJsonObject> location = MakeShareable(new FJsonObject);
	location->SetNumberField("X", loc.X);
	location->SetNumberField("Y", loc.Y);
	location->SetNumberField("Z", loc.Z);

	TSharedPtr<FJsonObject> rotation = MakeShareable(new FJsonObject);
	rotation->SetNumberField("X", rot.Pitch);
	rotation->SetNumberField("Y", rot.Yaw);
	rotation->SetNumberField("Z", rot.Roll);

	saveMap->Find(name)->ToSharedRef()->SetStringField("Name", name);
	saveMap->Find(name)->ToSharedRef()->SetObjectField("Location", location);
	saveMap->Find(name)->ToSharedRef()->SetObjectField("Rotation", rotation);
}

void JsonManager::Save()
{
	FString jsonStr = " ";
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonStr);

	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject);

	saveMap->GenerateKeyArray(myArr);

	for (int a = 0; a < saveMap->Num(); a++)
	{
		jsonObj->SetObjectField(myArr[a], saveMap->Find(myArr[a])->ToSharedRef());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myArr[a]);
	}

	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), jsonWriter);

	FFileHelper::SaveStringToFile(*jsonStr, *filePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

void JsonManager::LoadInit(TArray<FString> objs)
{

	FString loadStr = " ";
	FFileHelper::LoadFileToString(loadStr, *filePath);
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(loadStr);

	TSharedPtr<FJsonObject> readObj = MakeShareable(new FJsonObject);

	FJsonSerializer::Deserialize(jsonReader, readObj);
	
	for (int a=0; a<objs.Num(); a++)
	{
		loadMap->Add(objs[a], readObj->GetObjectField(objs[a]));		
	}
}

TSharedPtr<ObjectInfo> JsonManager::Load(FString name)
{
	if (loadMap->Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, name + "Array size is 0");
		return nullptr;
	}
	if (!loadMap->Find(name))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, name + "NotFound");
		return nullptr;
	}

	TSharedPtr<ObjectInfo> myInfo = MakeShareable(new ObjectInfo);

	TSharedPtr<FJsonObject> locObj = loadMap->Find(name)->Get()->GetObjectField("Location");
	TSharedPtr<FJsonObject> rotObj = loadMap->Find(name)->Get()->GetObjectField("Rotation");

	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, name);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, loadMap->Find(name)->Get()->GetObjectField(name)->GetStringField("Name"));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "End");*/

	FVector loc(locObj->GetNumberField("X"), locObj->GetNumberField("Y"), locObj->GetNumberField("Z"));
	FRotator rot(rotObj->GetNumberField("X"), rotObj->GetNumberField("Y"), rotObj->GetNumberField("Z"));

	myInfo->loc = loc;
	myInfo->rot = rot;
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myInfo->loc.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myInfo->rot.ToString());

	return myInfo;
}