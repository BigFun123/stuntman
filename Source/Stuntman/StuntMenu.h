// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PubSub/PubSub.h"
#include "StuntMenu.generated.h"

// create a delegate for the start button
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartClicked);

/**
 * Main Menu for the Stuntman app
 */
UCLASS()
class STUNTMAN_API UStuntMenu : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnNewClicked();
	
	UFUNCTION(BlueprintCallable, Category=UI)
	void OnStartClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnStopClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnResetClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnSaveAsStartupClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnLoadStartupClicked();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnLoad();
	
	UFUNCTION(BlueprintCallable, Category=UI)
	void OnSave();

	UFUNCTION(BlueprintCallable, Category=UI)
	void NextTake();

	UFUNCTION(BlueprintCallable, Category=UI)
	void PrevTake();

	UFUNCTION(BlueprintCallable, Category=UI)
	void OnTestClicked(const FString& data);

public:
	UPROPERTY(EditAnywhere, Category=UI)
	int Scene = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	int Take = 1;
	
	virtual bool Initialize() override;
};
