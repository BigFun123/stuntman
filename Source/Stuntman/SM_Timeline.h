// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SM_Timeline.generated.h"

/**
 * 
 */
UCLASS()
class STUNTMAN_API USM_Timeline : public UUserWidget
{
	GENERATED_BODY()
public:
	float time = 0;

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void SetFrameTime(float NewTime);
protected:
	UFUNCTION(BlueprintCallable, Category = "Timeline")
	float GetFrameTime();	
	
};
