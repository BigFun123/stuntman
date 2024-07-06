// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_HumanActor.h"
#include "Recorder/Recorder.h"

// Sets default values
ASM_HumanActor::ASM_HumanActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASM_HumanActor::BeginPlay()
{
	if (!IsDirector) {
		Recorder::AddObject(this);
	}

	Super::BeginPlay();
	
}

void ASM_HumanActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Recorder::RemoveObject(this);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASM_HumanActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASM_HumanActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

