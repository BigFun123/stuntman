// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PubSub/PubSub.h"
#include "SM_SpawnFactory.generated.h"

UCLASS()
class STUNTMAN_API ASM_SpawnFactory : public AActor, public ISubscriber
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASM_SpawnFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void onMessage(PubSubMessage& payload) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnObject(FString ActorClassPath, UWorld* world);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void OnDestroy(AActor* actor);

};
