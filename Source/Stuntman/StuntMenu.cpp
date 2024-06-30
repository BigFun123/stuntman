// Fill out your copyright notice in the Description page of Project Settings.


#include "StuntMenu.h"

void UStuntMenu::OnStartClicked()
{	
	PubSub::Send("StartRecording");
	UE_LOG(LogTemp, Warning, TEXT("Start button clicked"));
}

void UStuntMenu::OnResetClicked()
{
	PubSub::Send("ResetRecording");
	UE_LOG(LogTemp, Warning, TEXT("Reset button clicked"));
}

void UStuntMenu::OnSaveAsStartupClicked()
{
	PubSub::Send("SaveStartup");
	UE_LOG(LogTemp, Warning, TEXT("Save button clicked"));
}

bool UStuntMenu::Initialize()
{	
	bool result = Super::Initialize();
	PubSub::Send("Initialize");
	return result;
}