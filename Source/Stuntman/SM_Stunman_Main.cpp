// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Stuntman_Main.h"
#include "PubSub/PubSub.h"
#include "Recorder/RecorderConstants.h"

// Sets default values
ASM_Stuntman_Main::ASM_Stuntman_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASM_Stuntman_Main::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASM_Stuntman_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASM_Stuntman_Main::SpawnObject(FString ActorClassPath, UWorld* world)
{
	PubSubMessage pm;
	pm.message = SM_SPAWN;
	pm.spayload = ActorClassPath;
	pm.opayload = world;
	PubSub::Send(pm);
}

void ASM_Stuntman_Main::onMessage(PubSubMessage& payload)
{
	if (payload.message == SM_SPAWN) {
		FString ActorClassPath = payload.spayload;
		UWorld* world = (UWorld*)payload.opayload;
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector location = FVector(0, 0, 0);
		FRotator rotation = FRotator(0, 0, 0);
		FActorSpawnParameters params;
		//AActor* pA = world->SpawnActor<AActor>(FString::Printf(TEXT("%s"), *ActorClassPath), location, rotation, params);
	}
	if (payload.message == SM_DESTROY) {
		AActor* pA = (AActor*)payload.opayload;
		pA->Destroy();
	}
}

void ASM_Stuntman_Main::OnDestroy(AActor* actor)
{
	PubSubMessage pm;
	pm.message = SM_DESTROY;
	pm.opayload = actor;
	PubSub::Send(pm);
}

