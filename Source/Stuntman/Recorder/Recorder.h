// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <unordered_map>
#include "RecorderEvent.h"
#include "CoreMinimal.h"
#include "../PubSub/Subscriber.h"

/**
 * Event Recorder that streams JSONL objects
 *  public FTickableGameObject,
 */
class STUNTMAN_API Recorder : public ISubscriber
{
public:		
	std::unordered_map<AActor*, std::vector<FRecorderEvent>> recordings;
	bool Recording = false;	
	//float Time = 0;
	int Frame = 0;	
	int Scene = 1;
	int Take = 1;	
	
	FString SceneName = "Scene";
	//void Tick(float DeltaTime) override;
	void GotoFrame(int Frame);
	void NextTake();
	void PrevTake();
	void AddEvent(AActor* object, int event = 0);
	void RecordFrame();
	void NewScene();
	void Save();
	void Load();

	Recorder();
	virtual ~Recorder();
	static Recorder& GetInstance();
	float Round2(float f);
	
	void LogText();
	void LogJSON(const FString& name);
	void StartRecording();
	void StopRecording();
	void SaveStartup();
	void LoadStartup();
	void onMessage(PubSubMessage& message) override;
	void SpawnObject(FString ActorClassPath, UWorld* world);

	bool IsBarrel(AActor* object);
	void DetonateObjects(bool bDetonate);

	// use these in Blueprint constructors to add and remove. note: you MUST implement RemoveObject in the destructor
	static void AddObject(AActor* object);
	static void RemoveObject(AActor* object);
	
	static int GetTake();
	static int GetScene();
};
