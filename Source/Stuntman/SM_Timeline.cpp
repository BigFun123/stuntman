// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Timeline.h"
#include "PubSub/PubSub.h"
#include "Recorder/RecorderConstants.h"

void USM_Timeline::GotoFrame(float NewFrame)
{
    Frame = NewFrame;
    PubSubMessage pm;
    pm.message = SM_GOTOFRAME;
    pm.ipayload = Frame;
    PubSub::Send(pm);
}

float USM_Timeline::GetFrame()
{
    return Frame;
}

void USM_Timeline::StartRecording()
{
    Recording = true;
    PubSubMessage pm;
    pm.message = SM_START;    
	GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::Printf(TEXT("Start Scene 1 Frame %d"), Frame));
    PubSub::Send(pm);
}

void USM_Timeline::StopRecording()
{
    Recording = false;
    PubSubMessage pm;
    pm.message = SM_STOP;
    GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::Printf(TEXT("Stop Scene 1 Frame %d"), Frame));
    PubSub::Send(pm);
}

void USM_Timeline::StartPlaying()
{
    Recording = false;
    Playing = true;
    PubSubMessage pm;
    pm.message = SM_PLAYSTART;
    GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::Printf(TEXT("Stop Scene 1 Frame %d"), Frame));
    PubSub::Send(pm);
}

void USM_Timeline::StopPlaying()
{
    Playing = false;
    Recording = false;
    PubSubMessage pm;
    pm.message = SM_PLAYSTOP;    
	GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::Printf(TEXT("Start Scene 1 Frame %d"), Frame));
    PubSub::Send(pm);
}

void USM_Timeline::Detonate(bool bDetonate)
{
    PubSubMessage pm;
    pm.message = SM_DETONATE;
    pm.bpayload = bDetonate;
    pm.ipayload = Frame; 
    pm.ipayload2 = 1; // detonation group
    PubSub::Send(pm);
}

void USM_Timeline::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    GEngine->AddOnScreenDebugMessage(0, 0, FColor::Red, FString::Printf(TEXT("Scene 1 Frame %d"), Frame));
    
    // todo add time to event
    if (Recording)
    {
        PubSubMessage pm;        
        pm.ipayload = Frame;
        pm.message = SM_RECORDFRAME;        
        PubSub::Send(pm);
        Frame++;
    } 

    if (Playing)
    {
        PubSubMessage pm;                
        pm.ipayload = Frame;
        pm.message = SM_GOTOFRAME;
        PubSub::Send(pm);
        Frame++;
    }

    
    //Time += DeltaTime;
}