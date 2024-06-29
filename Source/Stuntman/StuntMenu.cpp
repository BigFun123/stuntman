// Fill out your copyright notice in the Description page of Project Settings.


#include "StuntMenu.h"

void UStuntMenu::OnStartClicked()
{
	// Start the game
	//log
	PubSub::Send("StartRecording");
	UE_LOG(LogTemp, Warning, TEXT("Start button clicked"));
}

void UStuntMenu::OnResetClicked()
{
	// Reset the game
	//log
	PubSub::Send("ResetRecording");
	UE_LOG(LogTemp, Warning, TEXT("Reset button clicked"));
}

void UStuntMenu::OnSaveAsStartupClicked()
{
	PubSub::Send("SaveStartup");
	UE_LOG(LogTemp, Warning, TEXT("Save button clicked"));
}
