// Fill out your copyright notice in the Description page of Project Settings.

#include "SM_BoxActor.h"
#include "Recorder/Recorder.h"
#include "Kismet/GameplayStatics.h"
#include "SM_Gizmo.h"
#include "UObject/ConstructorHelpers.h" // Add this line
// #include "Components/StaticMeshComponent.h"
// #include "Components/BoxComponent.h"

// Sets default values
ASM_BoxActor::ASM_BoxActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// UObject* pO = GetDefaultSubObject<UObject>(TEXT("Blueprint_Effect_Fire"));

	//	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	//	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//	BoxCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	//	BoxCollision->SetupAttachment(BoxMesh);

	// get player controller, enable click event
}

// Called when the game starts or when spawned
void ASM_BoxActor::BeginPlay()
{

	/*
	UObject* pO = GetDefaultSubobjectByName(TEXT("Blueprint_Effect_Fire"));
	if (pO) {
		UE_LOG(LogTemp, Warning, TEXT("Found Blueprint_Effect_Fire"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Did not find Blueprint_Effect_Fire"));
	}

*/
	// only add if we're not an editor object
	if (!HasAnyFlags(RF_Transient))
	{
		Recorder::AddObject(this);
		OnClicked.AddDynamic(this, &ASM_BoxActor::OnClickedHandler);		
		APlayerController *pPC = GetWorld()->GetFirstPlayerController();
		//FString path = FName(*this->GeneratedClass());
		//UE_LOG(LogTemp, Warning, path);
		pPC->bEnableClickEvents = true;
		pPC->bShowMouseCursor = true;
		pPC->bEnableMouseOverEvents = true;
		// pPC->bEnableTouch = true;
		pPC->bEnableTouchEvents = true;
		pPC->bEnableTouchOverEvents = true;
		pPC->bEnableTouchOverEvents = true;
	}
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

void ASM_BoxActor::Detonate(bool bDetonate)
{
	UE_LOG(LogTemp, Warning, TEXT("Detonate"));
	TArray<UObject *> pOA;
	GetDefaultSubobjects(pOA);
	UE_LOG(LogTemp, Warning, TEXT("Found %d components by tag"), pOA.Num());

	FString tag = "Blueprint_Effect_Fire";

	if (pOA.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d attached actors"), pOA.Num());
		for (UObject *pA : pOA)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found attached actor %s"), *pA->GetName());
			if (pA->GetName().Equals(tag, ESearchCase::IgnoreCase))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Blueprint_Effect_Fire"));
				USceneComponent *pSC = Cast<USceneComponent>(pA);
				pSC->SetVisibility(bDetonate);
				// pA->SetVisible(true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not find Blueprint_Effect_Fire"));
			}
			// log tags
			// for (FName n : pA->tags) {
			//	UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *n.ToString());
			//}
			// if (pA->Tags(TEXT("Fire"))) {
			//	UE_LOG(LogTemp, Warning, TEXT("Found Blueprint_Effect_Fire"));
			//}
			// else {
			//	UE_LOG(LogTemp, Warning, TEXT("Did not find Blueprint_Effect_Fire"));
			//}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not find attached actors"));
	}
}

void ASM_BoxActor::OnClickedHandler(AActor *Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
	// find SM_Gizmo_BP in scene
	// if found, set visibility to true
	// if not found, spawn SM_Gizmo_BP
	// if found, set location to this location
	// if not found, spawn SM_Gizmo_BP
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
				pG->AttachTo(this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not find Gizmo"));
			}
			UE_LOG(LogTemp, Warning, TEXT("Found actor %s"), *pA->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not find world"));
	}


}
