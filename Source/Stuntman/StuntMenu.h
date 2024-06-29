// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PubSub/PubSub.h"
#include "StuntMenu.generated.h"

// create a delegate for the start button
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartClicked);

/**
 * 
 */
UCLASS()
class STUNTMAN_API UStuntMenu : public UUserWidget
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category=UI)
	void OnStartClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnResetClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnSaveAsStartupClicked();
};
