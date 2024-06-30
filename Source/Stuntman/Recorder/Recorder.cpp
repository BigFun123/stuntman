// Fill out your copyright notice in the Description page of Project Settings.

#include "Recorder.h"
#include "../PubSub/PubSub.h"
#include "Serialization/JsonSerializer.h"

Recorder &Recorder::GetInstance()
{
	static Recorder _instance;
	return _instance;
}

Recorder::Recorder()
{
	PubSub::Subscribe(this);
}

Recorder::~Recorder()
{
	PubSub::Unsubscribe(this);
	StopRecording();
}

void Recorder::Tick(float DeltaTime)
{
	FColor color = FColor::Green;
	if (Recording)
	{
		Time += DeltaTime;
		Counter++;
		FString SaveDirectory = FPaths::ProjectSavedDir();
		FString FileName = FString::Printf(TEXT("Scene_%d_Take_%d.jsonl"), Scene, Take);
		FString AbsoluteFilePath = SaveDirectory + FileName;
		LogJSON(AbsoluteFilePath);
	}
	else
	{
		color = FColor::Red;
	}

	FString Message = FString::Printf(TEXT("Scene 1 Take %d     Frame %d"), Take, Counter);
	GEngine->AddOnScreenDebugMessage(0, 0, color, Message);
}

TStatId Recorder::GetStatId() const
{
	return TStatId();
}

void Recorder::onMessage(const FName &message)
{
	if (message == "Initialize") {
		Counter = 0;
		Time = 0;
		Take = 0;		
		SceneName = "Scene";
		StopRecording();
	}
	if (message == "SaveStartup") {
		SaveStartup();
	}
	if (message == "ResetRecording")
	{
		Counter = 0;
		Time = 0;
		Take = 0;		
		SceneName = "Scene";
		StopRecording();
	}
	if (message == "StartRecording")
	{
		if (Recording == false)
		{
			StartRecording();
		}
		else
		{
			StopRecording();
		}
	}
	else if (message == "StopRecording")
	{
		StopRecording();
	}
}

void Recorder::AddObject(const AActor *object)
{

	// only add if it's not already in the list
	for (int i = 0; i < GetInstance().objects.size(); i++)
	{
		if (GetInstance().objects[i] == object)
		{
			return;
		}
	}

	GetInstance().objects.push_back(object);
}

void Recorder::RemoveObject(const AActor *object)
{
	std::vector<const AActor *> &objects = GetInstance().objects;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == object)
		{
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

void Recorder::StartRecording()
{
	Counter = 0;
	Time = 0;
	Take++;
	Recording = true;
}

void Recorder::StopRecording()
{
	Recording = false;
}

void Recorder::SaveStartup()
{
	// save the position of all objects
	FString SaveDirectory = FPaths::ProjectSavedDir();
	FString FileName = FString::Printf(TEXT("Scene_%d_Startup.jsonl"), Scene, Take);
	FString AbsoluteFilePath = SaveDirectory + FileName;
	FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_None);
	LogJSON(AbsoluteFilePath);
}

void Recorder::LogJSON(const FString& AbsoluteFilePath)
{
	FString output;

	for (int i = 0; i < objects.size(); i++)
	{

		const AActor *object = objects[i];
		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}
		// Get the current time
		float CurrentTime = object->GetWorld()->GetTimeSeconds();
		// get the pawn world position
		FVector PawnLocation = objects[i]->GetActorLocation();
		FVector PawnRotation = objects[i]->GetActorRotation().Euler();
		// print to the console
		// UE_LOG(LogTemplateVehicle, Log, TEXT("Pawn Location: %s"), *PawnLocation.ToString());
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pawn Location: %s"), *PawnLocation.ToString()));
		FString name = "\"name\":\"" + object->GetName() + "\", \"type\":\"actor\",";

		FString TextToSave = name + FString::Printf(TEXT("\"index\":%d,\"counter\":%d,\"time\":%f,\"globaltime\":%f,\"x\": %f,\"y\":%f,\"z\":%f,\"rx\":%f,\"ry\":%f,\"rz\":%f"),
													i,
													Counter,
													Time,
													CurrentTime,
													PawnLocation.X, PawnLocation.Y, PawnLocation.Z,
													PawnRotation.X, PawnRotation.Y, PawnRotation.Z);

		

		output += "{" + TextToSave + "}\n";
	}

	
	FFileHelper::SaveStringToFile(output, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void Recorder::LogText()
{

	FString output;

	for (int i = 0; i < objects.size(); i++)
	{

		const AActor *object = objects[i];
		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}
		// Get the current time
		float CurrentTime = object->GetWorld()->GetTimeSeconds();
		// get the pawn world position
		FVector PawnLocation = objects[i]->GetActorLocation();
		FVector PawnRotation = objects[i]->GetActorRotation().Euler();
		// print to the console
		// UE_LOG(LogTemplateVehicle, Log, TEXT("Pawn Location: %s"), *PawnLocation.ToString());
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pawn Location: %s"), *PawnLocation.ToString()));
		FString name = object->GetName();

		FString TextToSave = name + FString::Printf(TEXT(" %d %d %f %f %f %f %f %f %f %f"),
													i,
													Counter,
													Time,
													CurrentTime,
													PawnLocation.X, PawnLocation.Y, PawnLocation.Z,
													PawnRotation.X, PawnRotation.Y, PawnRotation.Z);

		TextToSave += "\n";

		output += TextToSave;
	}

	FString SaveDirectory = FPaths::ProjectSavedDir();
	FString FileName = FString::Printf(TEXT("Scene_%d_Take_%d.txt"), Scene, Take);
	FString AbsoluteFilePath = SaveDirectory + FileName;
	FFileHelper::SaveStringToFile(output, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void Recorder::SetTime(float AbsoluteTime)
{
	Time = AbsoluteTime;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetTime: %f"), Time));
	// get the position of all objects at time, and set their current position to that
}
