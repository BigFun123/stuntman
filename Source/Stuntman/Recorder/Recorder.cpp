// Fill out your copyright notice in the Description page of Project Settings.

#include "Recorder.h"
// #include "Engine/World.h" // Add this line to include the header file for GetWorld()
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../PubSub/PubSub.h"
#include "RecorderConstants.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

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
		
		PubSubMessage pm;
		pm.message = SM_SETTIME;
		pm.fpayload = Time;
		PubSub::send(pm)
		Counter++;
		AddEvents();	
	}
	else
	{
		color = FColor::Red;
	}

	FString Message = FString::Printf(TEXT("Scene 1 Take %d     Frame %d"), Take, Counter);
	GEngine->AddOnScreenDebugMessage(0, 0, color, Message);
}

void Recorder::AddEvents()
{
	// add an even for each object in the recordings
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;
		const std::vector<FRecorderEvent> events = recording.second;

		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}

		AddEvent(object);
	}
}

float Recorder::Round2(float f)
{
	// route to 2 decimal places
	int fi = FMath::RoundToInt(f * 100.0);
	float rf = fi / 100.0;
	return rf;
}

void Recorder::AddEvent(AActor *actor)
{
	FRecorderEvent event;
	event.Name = actor->GetName();
	event.Time = Time;
	event.Position = actor->GetActorLocation();
	event.Type = actor->GetClass()->GetName();
	// route to 2 decimal places
	event.Position.X = event.Position.X;
	event.Position.Y = event.Position.Y;
	event.Position.Z = event.Position.Z;

	event.Rotation = actor->GetActorRotation().Euler();
	// route to 2 decimal places
	event.Rotation.X = event.Rotation.X;
	event.Rotation.Y = event.Rotation.Y;
	event.Rotation.Z = event.Rotation.Z;

	event.Velocity = actor->GetVelocity();
	// route to 2 decimal places
	event.Velocity.X = event.Velocity.X;
	event.Velocity.Y = event.Velocity.Y;
	event.Velocity.Z = event.Velocity.Z;

	// event.AngularVelocity = actor->GetActorAngularVelocity();
	event.Scene = Scene;

	// std::vector<FRecorderEvent> events = recordings[actor];
	// events.push_back(event);
	recordings[actor].push_back(event);
}

TStatId Recorder::GetStatId() const
{
	return TStatId();
}

void Recorder::onMessage(PubSubMessage &payload)
{
	if (payload.message == SM_NEW)
	{
		NewScene();
	}

	if (payload.message == SM_SETTAKE)
	{
		Take = payload.ipayload;
		Load();
	}
	if (payload.message == SM_SETTIME)
	{
		float time = payload.fpayload;
		SetTime(time);
	}
	if (payload.message == SM_INITIALIZING)
	{
		Counter = 0;
		Time = 0;
		Take = 0;
		SceneName = "Scene";
		StopRecording();
	}
	if (payload.message == SM_SAVESTARTUP)
	{
		SaveStartup();
	}
	if (payload.message == SM_RESET)
	{
		Counter = 0;
		Time = 0;
		Take = 0;
		SceneName = "Scene";
		StopRecording();
	}
	if (payload.message == SM_START)
	{
		StartRecording();
	}
	else if (payload.message == SM_STOP)
	{
		StopRecording();
		Save();
	}
	if (payload.message == SM_LOAD)
	{
		Load();
	}
}

void Recorder::NewScene()
{
	// stop recording
	StopRecording();
	// save the current take
	//Save();
	// reset the take
	Take = 1;

	/*// remove all objects from the scene
	// iterate all objects in the recordings map and remove them from the level
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;
		if (object != nullptr)
		{
			UWorld *world = object->GetWorld();
			if (world != nullptr)
			{
				//world->DestroyActor(object);
				PubSubMessage pm;
				pm.message = SM_DESTROY;
				pm.opayload = object;
				PubSub::Send(pm);
			}
		}
	}*/

// clear all recordings but keep the actors of type SM_HumanActor
 for (const auto &recording : recordings)
	{
		AActor *object = recording.first;
		if (object != nullptr)
		{
			if (object->GetClass()->GetName() != "SM_HumanActor")
			{
				recordings.erase(object);
			} else {
				// just clear the events
				recordings[object].clear();
			}
		}
	
	}

}

void Recorder::SpawnObject(FString ActorClassPath, UWorld *world)
{
	UBlueprint *bp = LoadObject<UBlueprint>(nullptr, *ActorClassPath);
	// UBlueprint* bp = Cast<UBlueprint*>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *ActorClassPath));

	bool bUseEditorWorld = false;
	AActor *spawn = world->SpawnActor(bp->GeneratedClass, &FVector::ZeroVector, &FRotator::ZeroRotator);
	world->MarkPackageDirty();
}

void Recorder::AddObject(AActor *object)
{
	// check if the object exists in the recordings map
	// if it does, then add the current transform to the list
	// if it doesn't, then create a new list and add the current transform to the list
	std::unordered_map<AActor *, std::vector<FRecorderEvent>> &recordings = GetInstance().recordings;

	if (recordings.find(object) == recordings.end())
	{
		std::vector<FRecorderEvent> events;
		recordings[object] = events;
	}

	GetInstance().AddEvent(object);
}

void Recorder::RemoveObject(AActor *object)
{
	// remove the object from the recordings
	std::unordered_map<AActor *, std::vector<FRecorderEvent>> &recordings = GetInstance().recordings;
	recordings.erase(object);
}

void Recorder::StartRecording()
{
	Counter = 0;
	Time = 0;
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
	// LogJSON(AbsoluteFilePath);

	FString output;

	for (const auto &recording : recordings)
	{
		FString outputString;
		const AActor *object = recording.first;
		const std::vector<FRecorderEvent> events = recording.second;

		// get first event
		if (events.size() > 0)
		{
			FRecorderEvent event = events[0];
			// serialize event to string

			FJsonObjectConverter::UStructToJsonObjectString<FRecorderEvent>(event, outputString, 0, 0);
			// remove all "\r\n" from outputstring
			outputString = outputString.Replace(TEXT("\r\n"), TEXT(""));
			outputString = outputString.Replace(TEXT("\n"), TEXT(""));
			outputString = outputString.Replace(TEXT("\t"), TEXT(""));
			outputString = outputString.Replace(TEXT(": "), TEXT(":"));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));

			outputString += "\n";

			output += outputString;
		}
	}

	FFileHelper::SaveStringToFile(output, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void Recorder::Save()
{
	// save the recordings map to a JSONL file
	FString SaveDirectory = FPaths::ProjectSavedDir();
	FString FileName = FString::Printf(TEXT("Scene_%d_Take_%d.jsonl"), Scene, Take);
	FString AbsoluteFilePath = SaveDirectory + FileName;

	FString output;

	// for each object in the recordings, write to the file
	int i = 0;
	for (const auto &recording : recordings)
	{
		const AActor *object = recording.first;
		const std::vector<FRecorderEvent> events = recording.second;

		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}

		// for each event
		for (const auto &event : events)
		{
			// serialize event to string
			FString outputString;
			FJsonObjectConverter::UStructToJsonObjectString<FRecorderEvent>(event, outputString, 0, 0);
			// remove all "\r\n" from outputstring
			outputString = outputString.Replace(TEXT("\r\n"), TEXT(""));
			outputString = outputString.Replace(TEXT("\n"), TEXT(""));
			outputString = outputString.Replace(TEXT("\t"), TEXT(""));
			outputString = outputString.Replace(TEXT(": "), TEXT(":"));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));
			outputString = outputString.Replace(TEXT("  "), TEXT(""));

			//  add to output
			output += outputString + "\n";
		}
	}

	FFileHelper::SaveStringToFile(output, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_None);
}

void Recorder::LoadStartup()
{
}

void Recorder::Load()
{
	// load the recordings map from a JSONL file
	// read the json objects one by one and add each to the recordings based on the name (find the actory by name)
	FString SaveDirectory = FPaths::ProjectSavedDir();
	FString FileName = FString::Printf(TEXT("Scene_%d_Take_%d.jsonl"), Scene, Take);
	FString AbsoluteFilePath = SaveDirectory + FileName;
	// if file does not exist, return
	if (!FPaths::FileExists(AbsoluteFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File Does Not Exist: %s"), *AbsoluteFilePath);
		return;
	}

	FString output;
	FFileHelper::LoadFileToString(output, *AbsoluteFilePath);

	// parse the JSONL file
	TArray<FString> lines;
	output.ParseIntoArray(lines, TEXT("\n"), true);

	for (const auto &line : lines)
	{
		// parse the JSON object
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(line);
		// deserialize into a FRecorderEvent

		// find object by name
		AActor *object = nullptr;
		// add event to recordings

		FRecorderEvent event;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(line, &event, 0, 0) == true)
		{
			// get name
			FString name = event.Name;
			// find object by name in recordings
			for (auto &recording : recordings)
			{
				AActor *recordingObject = reinterpret_cast<AActor *>(recording.first);
				if (recordingObject->GetName() == name)
				{
					object = recordingObject;
					break;
				}
			}
			if (object != nullptr)
			{
				recordings[object].push_back(event);
			}
		};
	}
}

void Recorder::LogJSON(const FString &AbsoluteFilePath)
{
	FString output;

	// std::unordered_map<const AActor*, std::vector<FTransform>> &recordings = GetInstance().recordings;

	int i = 0;
	for (const auto &recording : recordings)
	{
		const AActor *object = recording.first;
		const std::vector<FRecorderEvent> events = recording.second;

		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}

		// Get the current time
		float CurrentTime = object->GetWorld()->GetTimeSeconds();

		// Get the pawn world position
		FVector PawnLocation = object->GetActorLocation();
		FVector PawnRotation = object->GetActorRotation().Euler();

		// Create the JSON string
		FString name = "\"name\":\"" + object->GetName() + "\", \"type\":\"actor\",";
		FString TextToSave = name + FString::Printf(TEXT("\"index\":%d,\"counter\":%d,\"time\":%f,\"globaltime\":%f,\"x\": %f,\"y\":%f,\"z\":%f,\"rx\":%f,\"ry\":%f,\"rz\":%f"),
													i++,
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

	/*FString output;

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
	*/
}

void Recorder::SetTime(float AbsoluteTime)
{
	Time = AbsoluteTime;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetTime: %f"), Time));
	// get the position of all objects at time, and set their current position to that
	// find the time in the
}

void Recorder::NextTake()
{
	// stop recording
	StopRecording();
	// save the current take
	Save();
	// increment the take
	Take++;
	Load();
}

void Recorder::PrevTake()
{
	// stop recording
	StopRecording();
	// save the current take
	Save();
	// increment the take
	if (Take > 1)
	{
		Take--;
	}

	Load();
}

int Recorder::GetTake()
{
	return GetInstance().Take;
}

int Recorder::GetScene()
{
	return GetInstance().Scene;
}