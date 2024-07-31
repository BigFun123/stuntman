// Fill out your copyright notice in the Description page of Project Settings.

#include "Recorder.h"
// #include "Engine/World.h" // Add this line to include the header file for GetWorld()
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../PubSub/PubSub.h"
#include "RecorderConstants.h"
#include "../SM_BoxActor.h"
#include "../SM_HumanActor.h"
#include "Serialization/JsonSerializer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void Recorder::Clear()
{
	recordings.clear();
}

void Recorder::ClearSelected()
{
	//SelectedObject = nullptr;
	// clear the events for the selected actor
	if (SelectedObject != nullptr)
	{
		recordings[SelectedObject].clear();
	}
}

void Recorder::onMessage(PubSubMessage &payload)
{
	if (payload.message == SM_DETONATE)
	{
		Frame = payload.ipayload;
		// RecordFrame();
		DetonateObjects(payload.bpayload);
		Save();
	}

	if (payload.message == SM_NEW)
	{
		NewScene();
	}

	if (payload.message == SM_SETTAKE)
	{
		Take = payload.ipayload;
		Load();
	}
	// if (payload.message == SM_SETTIME)
	//{
	//	float time = payload.fpayload;
	//	SetTime(time);
	// }
	if (payload.message == SM_GOTOFRAME)
	{
		GotoFrame(payload.ipayload);
	}

	if (payload.message == SM_RECORDFRAME)
	{
		Frame = payload.ipayload;
		RecordFrame();
	}

	if (payload.message == SM_INITIALIZING)
	{
		Frame = 0;
		Take = 0;
		SceneName = "Scene";
		StopRecording();
	}
	if (payload.message == SM_LOADSTARTUP)
	{
		LoadStartup();
	}
	if (payload.message == SM_SAVESTARTUP)
	{
		SaveStartup();
	}
	if (payload.message == SM_RESET)
	{
		Frame = 0;
		Take = 0;
		SceneName = "Scene";
		StopRecording();
	}

	if (payload.message == SM_PLAYSTART)
	{
		StartPlayback();
	}
	if (payload.message == SM_PLAYSTOP)
	{
		StopPlayback();
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
	if (payload.message == SM_SAVE)
	{
		Save();
	}
	if (payload.message == SM_SELECT) {
		SelectedObject = (AActor *)payload.opayload;
	}
	if (payload.message == SM_CLEARSELECTED) {
		ClearSelected();
	}
}

void Recorder::GotoFrame(int InFrame)
{	
	Frame = InFrame;
	// get the position of all objects at time, and set their current position to that
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;

		// UPrimitiveComponent *comp = Cast<UPrimitiveComponent>(object->GetRootComponent());
		// if (comp)
		//{
		//	comp->SetSimulatePhysics(false);
		// }

		const std::map<int, FRecorderEvent> &events = recording.second;

		auto eventIt = events.find(InFrame);
		if (eventIt != events.end())
		{
			const FRecorderEvent &event = eventIt->second;
			// UE_LOG(LogTemp, Warning, TEXT("Event: %d %s"), event->Frame, *event->Name);

			if ((event.Event == SM_EVT_DETONATE) && IsBarrel(object))
			{
				ASM_BoxActor *box = Cast<ASM_BoxActor>(object);
				if (box)
				{
					box->Detonate(true);
				}
			}

			if (event.Event == SM_EVT_EXTINGUISH && IsBarrel(object))
			{
				ASM_BoxActor *box = Cast<ASM_BoxActor>(object);
				if (box)
				{
					box->Detonate(false);
				}
			}
			// object->SetAllPhysicsPosition(event->Position);
			// object->SetAllPhysicsRotation(FRotator(event->Rotation.X, event->Rotation.Y, event->Rotation.Z));
			//  set the position of the object to the position of the event

			// if object is an SM_HumanActor, then move to location
			// log name
			// UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *object->GetClass()->GetName());

			if (object->GetClass()->GetName() == TEXT("BP_PlayableVector_C"))
			{
				// cast to SM_HumanActor
				ASM_HumanActor *human = Cast<ASM_HumanActor>(object);

				// get human location
				FVector humanLocation = human->GetActorLocation();
				float dist = FVector::Dist(humanLocation, event.Position);
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("dist: %f"), dist), false);
				
				// if new location is above old location , then jump
				if (event.Position.Z > (humanLocation.Z + 30)) 
				{
					human->Jump(); 
					object->SetActorLocation(event.Position, false, nullptr, ETeleportType::TeleportPhysics);
				}
				// check if actor is not on the ground
				// IsWalking
				// if ( human->GetCharacterMovement()->IsFalling() == false)
				else
				{
					
					if ((PreviousFrame > Frame) || (dist > 350))
					{
						object->SetActorRotation(event.Rotation, ETeleportType::TeleportPhysics);
						object->SetActorLocation(event.Position, false, nullptr, ETeleportType::TeleportPhysics);
						
					}
					else
					{
						human->GetCharacterMovement()->MaxWalkSpeed = 1600;
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(human->GetController(), event.Position);
					}
				}
			}
			else
			{
				object->SetActorRotation(event.Rotation, ETeleportType::TeleportPhysics);
				object->SetActorLocation(event.Position, false, nullptr, ETeleportType::TeleportPhysics);
			}
		}		
	}
	PreviousFrame = Frame;
}

void Recorder::RecordFrame()
{
	// add an even for each object in the recordings
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;

		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}

		AddEvent(object, 0, Frame);
	}
}

float Recorder::Round2(float f)
{
	// route to 2 decimal places
	int fi = FMath::RoundToInt(f * 100.0);
	float rf = fi / 100.0;
	return rf;
}

void Recorder::AddEvent(AActor *actor, int eventCode, int InFrame)
{
	FRecorderEvent RecEvent;
	RecEvent.Name = actor->GetName();
	RecEvent.Frame = InFrame;
	RecEvent.Event = eventCode;

	RecEvent.Position = actor->GetActorLocation();
	// RecEvent.Type = actor->GetClass()->GetName();
	RecEvent.Type = actor->GetClass()->GetPathName();
	RecEvent.Rotation = actor->GetActorRotation().Quaternion();
	RecEvent.Velocity = actor->GetVelocity();
	// RecEvent.AngularVelocity = actor->GetActorAngularVelocity();
	RecEvent.Scene = Scene;

	// if the RecEvent exists and the existing RecEvent has an RecEvent.Event > 0, keep the Event
	auto eventIt = recordings[actor].find(InFrame);
	if (eventIt != recordings[actor].end())
	{
		FRecorderEvent existingEvent = eventIt->second;
		if (existingEvent.Event > 0)
		{
			RecEvent.Event = existingEvent.Event;
		}
	}

	recordings[actor][InFrame] = RecEvent;
}

void Recorder::DetonateObjects(bool bDetonate)
{
	// for each object in the recordings, detonate
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;
		// if the object is of type SM_BoxActor, then detonate
		UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *object->GetClass()->GetName());
		if (IsBarrel(object))
		{
			// object->Detonate(bDetonate);
			// cast as SM_BoxActor
			AddEvent(object, bDetonate == true ? SM_EVT_DETONATE : SM_EVT_EXTINGUISH, Frame); // detonate event

			ASM_BoxActor *box = Cast<ASM_BoxActor>(object);
			if (box)
			{
				box->Detonate(bDetonate);
			}
		}
	}
}

void Recorder::NewScene()
{
	// stop recording
	StopRecording();
	// save the current take
	// Save();
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
			// if (object->GetClass()->GetName() != "SM_HumanActor")
			//{
			//	recordings.erase(object);
			// }
			// else
			{
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

// small bug: objects are being added at their spawn location, not their physics settled location
// all objects are added to frame 0, currently spawning during playback is not supported
void Recorder::AddObject(AActor *object)
{
	// check if the object exists in the recordings map
	// if it does, then add the current transform to the list
	// if it doesn't, then create a new list and add the current transform to the list
	std::unordered_map<AActor *, std::map<int, FRecorderEvent>> &recordings = GetInstance().recordings;

	// only add if it's not already there
	if (recordings.find(object) == recordings.end())
	{
		std::map<int, FRecorderEvent> events;
		recordings[object] = events;
	}

	GetInstance().AddEvent(object, 0, 0);
}

// remove the object from the recordings
void Recorder::RemoveObject(AActor *object)
{
	GetInstance().recordings.erase(object);
}

void Recorder::SimulatePhysics(bool bSimulate)
{
	for (const auto &recording : recordings)
	{
		AActor *object = recording.first;

		UPrimitiveComponent *comp = Cast<UPrimitiveComponent>(object->GetRootComponent());
		if (comp)
		{
			if (IsBoxActor(object))
			{
				comp->SetSimulatePhysics(bSimulate);
			}
		}
	}
}
void Recorder::StartRecording()
{
	Recording = true;
	SimulatePhysics(true);
}

void Recorder::StopRecording()
{
	Recording = false;
	SimulatePhysics(false);
}

void Recorder::StartPlayback()
{
	Recording = false;
	SimulatePhysics(true);
}

void Recorder::StopPlayback()
{
	Recording = false;
	SimulatePhysics(true); // sic - we want to be able to drive after / during playback
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
		const std::map<int, FRecorderEvent> &events = recording.second;

		// get first event
		if (events.size() > 0)
		{
			// FRecorderEvent& event = events[0];
			auto eventIt = events.find(0);
			if (eventIt != events.end())
			{
				FRecorderEvent event = eventIt->second;

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
		const std::map<int, FRecorderEvent> events = recording.second;

		if (object->HasActorBegunPlay() == false)
		{
			continue;
		}

		// for each event
		for (const auto &event : events)
		{
			// serialize event to string
			FString outputString;
			FJsonObjectConverter::UStructToJsonObjectString<FRecorderEvent>(event.second, outputString, 0, 0);
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
	// load the recordings map from a JSONL file
	// read the json objects one by one and add each to the recordings based on the name (find the actory by name)
	FString SaveDirectory = FPaths::ProjectSavedDir();
	FString FileName = FString::Printf(TEXT("Scene_%d_Startup.jsonl"), Scene, Take);
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
				recordings[object][event.Frame] = event;
			}
			else
			{
				// if the object is not found, spawn it
				// SpawnObject(event.Type, object->GetWorld());
				PubSubMessage message;
				message.message = SM_SPAWN;
				message.opayload = &event;
				PubSub::Send(message);
			}
		};
	}
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
				recordings[object][event.Frame] = event;
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
		const std::map<int, FRecorderEvent> *events = &recording.second;

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
		FString TextToSave = name + FString::Printf(TEXT("\"index\":%d,\"frame\":%d,\"globaltime\":%f,\"x\": %f,\"y\":%f,\"z\":%f,\"rx\":%f,\"ry\":%f,\"rz\":%f"),
													i++,
													Frame,
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

bool Recorder::IsBarrel(AActor *object)
{
	return (object->GetClass()->GetName() == TEXT("BP_Barrel_C"));
}

bool Recorder::IsBoxActor(AActor *object)
{
	// find if an object is of type SM_BoxActor or extends from SM_BoxActor
	return object->GetClass()->IsChildOf(ASM_BoxActor::StaticClass());
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