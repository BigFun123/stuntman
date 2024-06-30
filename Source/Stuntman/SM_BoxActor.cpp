// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_BoxActor.h"
#include "Recorder/Recorder.h"
#include "UObject/ConstructorHelpers.h" // Add this line
//#include "Components/StaticMeshComponent.h"
//#include "Components/BoxComponent.h"

// Sets default values
ASM_BoxActor::ASM_BoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	//UObject* pO = GetDefaultSubObject<UObject>(TEXT("Blueprint_Effect_Fire"));
	UObject* pO = GetDefaultSubobjectByName(TEXT("Blueprint_Effect_Fire"));
	if (pO) {
		UE_LOG(LogTemp, Warning, TEXT("Found Blueprint_Effect_Fire"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Did not find Blueprint_Effect_Fire"));
	}

//	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
//	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
//	BoxCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
//	BoxCollision->SetupAttachment(BoxMesh);

}

// Called when the game starts or when spawned
void ASM_BoxActor::BeginPlay()
{
	Recorder::AddObject(this);
	Super::BeginPlay();
}

void ASM_BoxActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Recorder::RemoveObject(this);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASM_BoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

