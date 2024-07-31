// Fill out your copyright notice in the Description page of Project Settings.

#include "SM_Gizmo.h"

// Sets default values
ASM_Gizmo::ASM_Gizmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASM_Gizmo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASM_Gizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASM_Gizmo::AttachTo(AActor *actor)
{
	// move to actor location
	SetActorLocation(actor->GetActorLocation());

	// disable actor physics
	/*actor->SetActorEnableCollision(false);
	// get actor root component
	UPrimitiveComponent *comp = Cast<UPrimitiveComponent>(actor->GetRootComponent());
	if (comp)
	{	
			comp->SetSimulatePhysics(false);
	}

	// check if Attached is still valid

	if (Attached) {
		// detach from previous actor
		Attached->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Attached = nullptr;	
	}
	// attach actor to gizmo
	actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	Attached = actor;

	// attach to actor
	// AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);*/
}
