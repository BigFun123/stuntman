// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SM_BoxActor.generated.h"

UCLASS()
class STUNTMAN_API ASM_BoxActor : public AActor
{
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties
	ASM_BoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Box")
	class UStaticMeshComponent* BoxMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Box")
	class UBoxComponent* BoxCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
