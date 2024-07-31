// Fill out your copyright notice in the Description page of Project Settings.

#include "SM_Recordable.h"
#include "Kismet/GameplayStatics.h"
#include "SM_Gizmo.h"
#include "Recorder/Recorder.h"
#include "Recorder/RecorderConstants.h"
#include "PubSub/PubSub.h"

// Sets default values for this component's properties
USM_Recordable::USM_Recordable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Selected
void USM_Recordable::Select()
{
	PubSubMessage pm;
	pm.message = SM_SELECT;
	pm.opayload = GetOwner();
	PubSub::Send(pm);

	// find the gizmo and move it
	auto *pWorld = GetWorld();
	if (pWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found world"));
		TArray<AActor *> pActors;
		UGameplayStatics::GetAllActorsOfClass(pWorld, ASM_Gizmo::StaticClass(), pActors);
		UE_LOG(LogTemp, Warning, TEXT("Found %d actors"), pActors.Num());
		for (AActor *pA : pActors)
		{
			// cast to SM_Gizmo
			ASM_Gizmo *pG = Cast<ASM_Gizmo>(pA);
			if (pG)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Gizmo"));
				pG->AttachTo(GetOwner());
			}
		}
	}
}

// Called when the game starts
void USM_Recordable::BeginPlay()
{
	if (!IsDirector && !HasAnyFlags(RF_Transient))
	{
		// get parent actor
		AActor *parent = GetOwner();
		Recorder::AddObject(parent);
		parent->OnClicked.AddDynamic(this, &USM_Recordable::OnClickedHandler);
	}

	Super::BeginPlay();
}

void USM_Recordable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// get parent actor
	AActor *parent = GetOwner();
	Recorder::RemoveObject(parent);

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void USM_Recordable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USM_Recordable::OnClickedHandler(AActor *Target, FKey ButtonPressed)
{
	this->Select();
}
