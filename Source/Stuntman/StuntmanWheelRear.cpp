// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuntmanWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UStuntmanWheelRear::UStuntmanWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}