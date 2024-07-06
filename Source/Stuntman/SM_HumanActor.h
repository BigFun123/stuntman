// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SM_HumanActor.generated.h"

UCLASS()
class STUNTMAN_API ASM_HumanActor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASM_HumanActor();

	UPROPERTY(EditAnywhere, Category = Settings)
	bool IsDirector = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
