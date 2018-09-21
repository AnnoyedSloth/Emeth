// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonManager.h"
#include <mutex>
#include <JsonSerializer.h>
#include <Json.h>
#include "Engine.h"

JsonManager *JsonManager::instance = nullptr;
static std::once_flag onceFlag;

JsonManager::JsonManager()
{
	jsonMap = new TMap<FString, TSharedPtr<FJsonObject>>();
	loadMap = new TMap<FString, TSharedPtr<FJsonObject>>();
	//JsonManager::GetInstance()->LoadInit();
	isDeserialized = false;
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


void JsonManager::Save(FString name, FVector loc, FRotator rot)
{
	if (jsonMap->Find(name) == nullptr)
	{
		TSharedPtr<FJsonObject> objInfo = MakeShareable(new FJsonObject);
		jsonMap->Add(name, objInfo);
	}

	FString jsonStr = " ";
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonStr);

	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject);

	TSharedPtr<FJsonObject> location = MakeShareable(new FJsonObject);
	location->SetNumberField("X", loc.X);
	location->SetNumberField("Y", loc.Y);
	location->SetNumberField("Z", loc.Z);

	TSharedPtr<FJsonObject> rotation = MakeShareable(new FJsonObject);
	rotation->SetNumberField("X", rot.Roll);
	rotation->SetNumberField("Y", rot.Pitch);
	rotation->SetNumberField("Z", rot.Yaw);

	jsonMap->Find(name)->ToSharedRef()->SetStringField("Name", name);
	jsonMap->Find(name)->ToSharedRef()->SetObjectField("Location", location);
	jsonMap->Find(name)->ToSharedRef()->SetObjectField("Rotation", rotation);


	jsonMap->GenerateKeyArray(myArr);

	for (int a = 0; a < jsonMap->Num(); a++)
	{
		jsonObj->SetObjectField(myArr[a], jsonMap->Find(myArr[a])->ToSharedRef());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myArr[a]);
	}

	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), jsonWriter);

	FFileHelper::SaveStringToFile(*jsonStr, *filePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	isDeserialized = false;

}

void JsonManager::LoadInit()
{
	isDeserialized = true;

	FString loadStr = " ";
	FFileHelper::LoadFileToString(loadStr, *filePath);
	TSharedPtr<FJsonObject> readObj = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(loadStr);
	FJsonSerializer::Deserialize(jsonReader, readObj);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "StartPrinting");
	for (int a = 0; a < jsonMap->Num(); a++)
	{
		//readObj.ToSharedRef()->GetObjectField(myArr[a]);
		loadMap->Add(myArr[a], readObj);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, loadMap->Find(myArr[a])->Get()->GetObjectField(myArr[a])->GetStringField("Name"));
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myArr[a]);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "EndPrinting");

	//jsonObj = jsonMap->Find(readObj.ToSharedRef()->GetObjectField);


}

TSharedPtr<ObjectInfo> JsonManager::Load(FString name)
{
	{
		FScopeLock Lock(&criticalSection);
		if (!isDeserialized)
		{
			LoadInit();
		}
	}
	if (loadMap->Num() == 0) return nullptr;
	if (!loadMap->Find(name)) return nullptr;

	TSharedPtr<ObjectInfo> myInfo = MakeShareable(new ObjectInfo);

	TSharedPtr<FJsonObject> locObj = loadMap->Find(name)->Get()->GetObjectField(name)->GetObjectField("Location");
	TSharedPtr<FJsonObject> rotObj = loadMap->Find(name)->Get()->GetObjectField(name)->GetObjectField("Rotation");

	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, name);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, loadMap->Find(name)->Get()->GetObjectField(name)->GetStringField("Name"));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "End");*/

	//double num = loadMap->Find(name)->ToSharedRef()->GetObjectField("Location")->GetNumberField("X");

	//TSharedPtr<FJsonValue> x = locObj->Values

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, );

	FVector loc(locObj->GetNumberField("X"), locObj->GetNumberField("Y"), locObj->GetNumberField("Z"));
	FRotator rot(rotObj->GetNumberField("X"), rotObj->GetNumberField("Y"), rotObj->GetNumberField("Z"));

	myInfo->loc = loc;
	myInfo->rot = rot;
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myInfo->loc.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, myInfo->rot.ToString());

	return myInfo;

}