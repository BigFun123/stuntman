// Fill out your copyright notice in the Description page of Project Settings.

#include "StuntMenu.h"
#include "Recorder/RecorderConstants.h"


void UStuntMenu::OnNewClicked() 
{
	UWorld* w = GetWorld();
	PubSubMessage pm;
	pm.message = SM_NEW;
	pm.opayload = w;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("New button clicked"));
}

void UStuntMenu::OnStartClicked()
{
	PubSubMessage pm;
	pm.message = SM_START;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Start button clicked"));
}

void UStuntMenu::OnStopClicked()
{
	PubSubMessage pm;
	pm.message = SM_STOP;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Stop button clicked"));
}

void UStuntMenu::OnResetClicked()
{
	PubSubMessage pm;
	pm.message = SM_RESET;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Reset button clicked"));
}

void UStuntMenu::OnSaveAsStartupClicked()
{
	PubSubMessage pm;
	pm.message = SM_SAVESTARTUP;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Save startup button clicked"));
}

void UStuntMenu::OnLoad()
{
	PubSubMessage pm;
	pm.message = SM_LOAD;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Load button clicked"));
}

void UStuntMenu::OnSave()
{
	PubSubMessage pm;
	pm.message = SM_SAVE;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Save button clicked"));
}

bool UStuntMenu::Initialize()
{
	bool result = Super::Initialize();
	PubSubMessage pm;
	pm.message = SM_INITIALIZING;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Display, TEXT("Initialize"));
	return result;
}

void UStuntMenu::NextTake()
{
	PubSubMessage pm;
	pm.message = SM_SETTAKE;
	pm.ipayload = Take;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Next take button clicked"));
}

void UStuntMenu::PrevTake()
{
	PubSubMessage pm;
	pm.message = SM_SETTAKE;
	pm.ipayload = Take;
	PubSub::Send(pm);
	UE_LOG(LogTemp, Warning, TEXT("Prev take button clicked"));
}