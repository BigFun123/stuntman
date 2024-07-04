// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Timeline.h"
#include "PubSub/PubSub.h"
#include "Recorder/RecorderConstants.h"

void USM_Timeline::SetFrameTime(float NewTime)
{
    time = NewTime;

    PubSubMessage pm;
    pm.message = SM_SETTIME;
    pm.fpayload = time;
    PubSub::Send(pm);

}

float USM_Timeline::GetFrameTime()
{
    return time;
}