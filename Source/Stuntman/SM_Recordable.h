// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SM_Recordable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUNTMAN_API USM_Recordable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USM_Recordable();
	
	UPROPERTY(EditAnywhere, Category = Settings)
	bool IsDirector = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Select();

	UFUNCTION(BlueprintCallable, Category = "Box")
	void OnClickedHandler(AActor* Target, FKey ButtonPressed);


		
};
