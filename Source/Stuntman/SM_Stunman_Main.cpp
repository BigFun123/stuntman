// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Stuntman_Main.h"
#include "PubSub/PubSub.h"
#include "Recorder/Recorder.h"
#include "Recorder/RecorderEvent.h"
#include "Recorder/RecorderConstants.h"

// Sets default values
ASM_Stuntman_Main::ASM_Stuntman_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ASM_Stuntman_Main::~ASM_Stuntman_Main()
{
	PubSub::Unsubscribe(this);
}

// Called when the game starts or when spawned
void ASM_Stuntman_Main::BeginPlay()
{
	// instantiate Recorder
	Recorder SM_REC = Recorder::GetInstance();
	PubSub::Subscribe(this);
	Super::BeginPlay();	
}

void ASM_Stuntman_Main::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Recorder SM_REC = Recorder::GetInstance();
	SM_REC.Clear();
	PubSub::Unsubscribe(this);
	Super::EndPlay(EndPlayReason);
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
	if (payload.message == SM_TEST) {
		//FRecorderEvent* event = (FRecorderEvent*)payload.opayload;
		UWorld* world = GetWorld();		
		FString ActorClassPath = payload.spayload;
		UE_LOG(LogTemp, Warning, TEXT("ActorClassPath: %s"), *ActorClassPath);

		FActorSpawnParameters spawnParams;
		//spawnParams.Instigator = this;
		spawnParams.Owner = this;		
		// check if event->Name is null
		//spawnParams.Name = FName(*event->Name);
		
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector location = FVector(0,0,0);
		//create new rotator at 0,0,0
		FRotator rotation = FRotator(0,0,0);
		//AActor* pA = world->SpawnActor<AActor>(FString::Printf(TEXT("%s"), *ActorClassPath), location, rotation, params);
		UClass* actorClass = LoadObject<UClass>(NULL, *ActorClassPath);
		//try {
			AActor* pA = world->SpawnActor(actorClass, &location, &rotation, spawnParams);
		//} catch (const std::exception& e) {
		//	UE_LOG(LogTemp, Warning, TEXT("Exception: %s"), *FString(e.what()));
		//}

	}
	if (payload.message == SM_SPAWN) {
		FRecorderEvent* event = (FRecorderEvent*)payload.opayload;
		UWorld* world = GetWorld();		

		//FString ActorClassPath = payload.spayload;		
		// get ActorClassPath for blueprint by Blueprint name in event.Type
		FString ActorClassPath = FString(event->Type);
		UE_LOG(LogTemp, Warning, TEXT("ActorClassPath: %s"), *ActorClassPath);
		
		//FStringAssetReference itemRef = "Class'/Game/Blueprints/Items/Chest/chestBlue.chestBlue_C'"; UObject* itemObj = itemRef.ResolveObject(); AARArmor* item = GetWorld()->SpawnActor(itemObj->GetClass(),...

		FActorSpawnParameters spawnParams;
		//spawnParams.Instigator = this;
		spawnParams.Owner = this;
		spawnParams.Name = FName(*event->Name);
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector location = event->Position;
		FRotator rotation = FRotator(event->Rotation);
		//AActor* pA = world->SpawnActor<AActor>(FString::Printf(TEXT("%s"), *ActorClassPath), location, rotation, params);
		UClass* actorClass = LoadObject<UClass>(NULL, *ActorClassPath);
		//try {
			AActor* pA = world->SpawnActor(actorClass, &location, &rotation, spawnParams);
		//} catch (const std::exception& e) {
		//	UE_LOG(LogTemp, Warning, TEXT("Exception: %s"), *FString(e.what()));
		//}
		
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

