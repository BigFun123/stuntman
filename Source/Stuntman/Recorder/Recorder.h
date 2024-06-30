// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "../PubSub/Subscriber.h"

/**
 * Event Recorder that streams JSONL objects
 */
class STUNTMAN_API Recorder : public FTickableGameObject, public ISubscriber
{
public:	
	std::vector<const AActor*> objects;
	bool Recording = false;	
	float Time = 0;
	int Counter = 0;
	int Scene = 1;
	int Take = 0;	
	FString SceneName = "Scene";
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;
	void SetTime(float AbsoluteTime);

	Recorder();
	virtual ~Recorder();
	static Recorder& GetInstance();
	
	void LogText();
	void LogJSON(const FString& name);
	void StartRecording();
	void StopRecording();
	void SaveStartup();
	void onMessage(const FName& message);

	// use these in Blueprint constructors to add and remove. note: you MUST implement RemoveObject in the destructor
	static void AddObject(const AActor* object);
	static void RemoveObject(const AActor* object);
};
